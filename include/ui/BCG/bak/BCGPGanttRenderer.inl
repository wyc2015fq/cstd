
//
// BCGPGanttRenderer.cpp: implementation of the CBCGPGanttRenderer class.
//
//////////////////////////////////////////////////////////////////////

#include "BCGPGanttRenderer.h"
#include "BCGPGanttChart.h"
#include "BCGPGanttItem.h"
#include "BCGPDrawManager.h"

#if !defined (BCGP_EXCLUDE_GRID_CTRL) && !defined (BCGP_EXCLUDE_GANTT)

IMPLEMENT_DYNAMIC(CBCGPGanttRenderer, CObject)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPGanttRenderer::CBCGPGanttRenderer()
{

}

CBCGPGanttRenderer::~CBCGPGanttRenderer()
{

}

static void FillBar (const CBCGPGanttItem* pItem, CDC& dc, const CRect& rcFill, COLORREF color, double dGlowLine = 0.6f)
{
    CBCGPVisualManager::GetInstance()->FillGanttBar (pItem, dc, rcFill, color, dGlowLine);
}

void CBCGPGanttRenderer::DrawShadow (CBCGPGanttDrawContext& ctxDraw)
{
	if (ctxDraw.m_Item.IsSelected())
	{
		return;
	}
	CRect rectShadow = ctxDraw.m_rectBar;
	COLORREF clrShadow = ctxDraw.m_clrFill;
	if (ctxDraw.m_Item.IsGroupItem())
	{
		rectShadow.OffsetRect (4, 3);

        CRect rectOutput;
        rectOutput.IntersectRect (ctxDraw.m_rectClip, rectShadow);
        if (rectOutput.IsRectEmpty())
        {
            return;
        }

		CBCGPAlphaDC alphaDC(ctxDraw.m_DC, rectOutput, 0.12f);
		rectShadow.InflateRect (0, 0, 1, 1);
		alphaDC.BeginPath();
		DrawGroupPath (alphaDC, rectShadow);
		alphaDC.CloseFigure();
		alphaDC.EndPath();

		CBCGPBrushSelector brush (alphaDC, clrShadow);
		alphaDC.FillPath();
		return;
	}
	if (ctxDraw.m_Item.IsMileStone())
	{
		rectShadow.OffsetRect (4, 2);

        CRect rectOutput;
        rectOutput.IntersectRect (ctxDraw.m_rectClip, rectShadow);
        if (rectOutput.IsRectEmpty())
        {
            return;
        }

        CBCGPAlphaDC alphaDC(ctxDraw.m_DC, rectOutput, 0.12f);
		CBCGPBrushSelector brush (alphaDC, clrShadow);
		CBCGPPenSelector pen(alphaDC, clrShadow);
		DrawMileStonePolygon (alphaDC, rectShadow);
		return;
	}

	rectShadow.OffsetRect (-1, -1);
	if (rectShadow.Width() >= 7 && rectShadow.Height() >= 7)
	{
		CBCGPDrawManager dm (ctxDraw.m_DC);
		dm.DrawShadow (rectShadow, 5, 100, 64);
	}
}

void CBCGPGanttRenderer::DrawBar (CBCGPGanttDrawContext& ctxDraw)
{
	if (ctxDraw.m_Item.IsMileStone())
	{
		DrawMileStoneItem (ctxDraw);
	}
	else if (ctxDraw.m_Item.IsGroupItem())
	{
		DrawGroupItem (ctxDraw);
	}
	else
	{
		DrawSimpleItem (ctxDraw);
	}
}

void CBCGPGanttRenderer::DrawSelection (CBCGPGanttDrawContext& ctxDraw)
{
	int k = ctxDraw.m_Item.IsMileStone() ? 4 : 3;
	CRect rectSel = ctxDraw.m_rectBar;
	rectSel.InflateRect (k, k, k, k);

	int d = min (10, rectSel.Height());
	CPoint pt (d, d);

	CRect rectOutput;
	rectOutput.IntersectRect (ctxDraw.m_rectClip, rectSel);
	if (rectOutput.IsRectEmpty())
	{
		return;
	}

	CBCGPAlphaDC dc (ctxDraw.m_DC, rectOutput, 0.37f);
	CBCGPPenSelector pen(dc, ctxDraw.m_clrBorder);
	CBCGPBrushSelector brush(dc, ctxDraw.m_clrFill);
	if (ctxDraw.m_Item.IsMileStone())
	{
		IPOINT ptCenter = rectSel.CenterPoint();
		int d = rectSel.Height() / 2 - 1;
		IPOINT pts[4];

		pts [0] = ptCenter;
		pts [1] = ptCenter;
		pts [2] = ptCenter;
		pts [3] = ptCenter;

		pts[0].x -= d;
		pts[1].y -= d;
		pts[2].x += d;
		pts[3].y += d;
		dc.Polygon (pts, 4);
	}
	else
	{
		dc.RoundRect (&rectSel, pt);
	}
}

void CBCGPGanttRenderer::DrawSimpleItem (CBCGPGanttDrawContext& ctxDraw)
{
	COLORREF clrPrimary = ctxDraw.m_clrFill;
	COLORREF clrComplete = ctxDraw.m_clrFill2;
	COLORREF clrBorder = ctxDraw.m_clrBorder;

	CBCGPPenSelector penBorder (ctxDraw.m_DC, clrBorder);

	CRect rectBar = ctxDraw.m_rectBar;
	rectBar.DeflateRect (1, 1, 1, 1);

	int progressWidth = (int)(rectBar.Width() * ctxDraw.m_Item.GetProgress());

	bool bFillProgress = (progressWidth > 1);
	bool bFillBar = true;

	if (progressWidth <= 1 && ctxDraw.m_Item.GetProgress() > 0.5f)
	{
		// if there's a little space and the progress is high, fill the whole bar as complete
		bFillProgress = true;
		bFillBar = false;
	}

	if (progressWidth >= rectBar.Width() - 1)
	{
		bFillProgress = true;
		bFillBar = false;
	}

	if (bFillProgress && bFillBar)
	{
		CRect rectComplete = rectBar;
		rectComplete.r = rectComplete.l + progressWidth;
		FillBar (&ctxDraw.m_Item, ctxDraw.m_DC, rectComplete, clrComplete);
		ctxDraw.m_DC.MoveTo (rectComplete.r, rectBar.t);
		ctxDraw.m_DC.LineTo (rectComplete.r, rectBar.b);
		rectBar.l = rectComplete.r + 1;
		FillBar (&ctxDraw.m_Item, ctxDraw.m_DC, rectBar, clrPrimary);
	}
	else // either bFillProgress, or bFillBar is true
	{
		// Fill whole bar
		FillBar (&ctxDraw.m_Item, ctxDraw.m_DC, rectBar, bFillProgress ? clrComplete : clrPrimary);
	}

	CPoint pt (6, 6);
	CBrush* pOldBrush = (CBrush*)ctxDraw.m_DC.SelectStockObject (NULL_BRUSH);
	ctxDraw.m_DC.RoundRect (ctxDraw.m_rectBar, pt);
	ctxDraw.m_DC.SelectObject (pOldBrush);
}

void CBCGPGanttRenderer::DrawGroupPath (CDC& dc, CRect rectBar)
{
	rectBar.DeflateRect (0, 1, 0, 1);

	int y1 = (rectBar.t + rectBar.b) / 2;
	int x1 = rectBar.l + (rectBar.b - y1);
	int x2 = rectBar.r - (rectBar.b - y1);

	while (x1 >= x2 && y1 < rectBar.b)
	{
		x1 --;
		x2 ++;
		y1 ++;
	}

	dc.MoveTo (rectBar.l, rectBar.b);

	if (y1 < rectBar.b && x1 < x2)
	{
		dc.LineTo (x1, y1);
		dc.LineTo (x2, y1);
	}

	dc.LineTo (rectBar.r, rectBar.b);
	dc.LineTo (rectBar.r, rectBar.t);
	dc.LineTo (rectBar.l, rectBar.t);
    dc.LineTo (rectBar.l, rectBar.b);
}

void CBCGPGanttRenderer::DrawGroupItem (CBCGPGanttDrawContext& ctxDraw)
{
	ctxDraw.m_DC.BeginPath();
	DrawGroupPath (ctxDraw.m_DC, ctxDraw.m_rectBar);
	ctxDraw.m_DC.CloseFigure();
	ctxDraw.m_DC.EndPath();

	CRgn rgn;
	rgn.CreateRectRgn (0, 0, 0, 0);
	CDC* hdc = ctxDraw.m_DC.GetSafeHdc();
	::GetClipRgn(hdc, (HRGN)rgn.GetSafeHandle());
	::SelectClipPath (hdc, RGN_AND);
	COLORREF clrFill = ctxDraw.m_clrFill;
	FillBar (&ctxDraw.m_Item, ctxDraw.m_DC, ctxDraw.m_rectBar, clrFill, 0.36f);
	::SelectClipRgn (hdc, (HRGN)rgn.GetSafeHandle());

    CBCGPPenSelector (ctxDraw.m_DC,ctxDraw.m_clrBorder);
    DrawGroupPath (ctxDraw.m_DC, ctxDraw.m_rectBar);   
}

void CBCGPGanttRenderer::DrawMileStonePolygon (CDC& dc, CRect rectBar)
{
	IPOINT ptCenter = rectBar.CenterPoint();
	int d = rectBar.Height() / 2 - 1;

	IPOINT pts[4];
	pts [0] = ptCenter;
	pts [1] = ptCenter;
	pts [2] = ptCenter;
	pts [3] = ptCenter;

	pts[0].x -= d;
	pts[1].y -= d;
	pts[2].x += d;
	pts[3].y += d;
	dc.Polygon (pts, 4);
}

void CBCGPGanttRenderer::DrawMileStoneItem (CBCGPGanttDrawContext& ctxDraw)
{
	CBCGPPenSelector pen (ctxDraw.m_DC, ctxDraw.m_clrBorder);
    CBCGPBrushSelector brush (ctxDraw.m_DC, ctxDraw.m_Item.IsCompleted() ? ctxDraw.m_clrFill2 : ctxDraw.m_clrFill);
	DrawMileStonePolygon (ctxDraw.m_DC, ctxDraw.m_rectBar);
}

void CBCGPGanttRenderer::DrawConnection (CBCGPGanttDrawContext& ctxDraw, const CBCGPGanttConnection& link)
{
	POSITION pos = link.m_Points.GetHeadPosition();
	if (pos == NULL)
	{
		return;
	}

	CPoint ptOrigin = ctxDraw.m_rectBar.TopLeft();
	POSITION posLast = link.m_Points.GetTailPosition();
    BOOL bFirst = TRUE;
    CPoint ptBegin = link.m_Points.GetNext (pos);

    CBCGPPenSelector pen (ctxDraw.m_DC, ctxDraw.m_clrFill);
	CBCGPBrushSelector brush (ctxDraw.m_DC, ctxDraw.m_clrFill);

    while (pos != NULL)
	{
		BOOL bLast = (pos == posLast);
		CPoint ptEnd = link.m_Points.GetNext (pos);

		DrawConnectionLine (ctxDraw, (ptBegin + ptOrigin), (ptEnd + ptOrigin), bFirst, bLast);

		ptBegin = ptEnd;
		bFirst = FALSE;
	}
}

void CBCGPGanttRenderer::DrawConnectionLine (CBCGPGanttDrawContext& ctxDraw, CPoint ptBegin, CPoint ptEnd, BOOL bFirst, BOOL bLast)
{
	double distSqr = (ptBegin.x - ptEnd.x) * (ptBegin.x - ptEnd.x) + (ptBegin.y - ptEnd.y) * (ptBegin.y - ptEnd.y);

	if (distSqr < 1.0)
	{
		return;
	}

	ctxDraw.m_DC.MoveTo (ptEnd);
	ctxDraw.m_DC.LineTo (ptBegin);

	if (distSqr > 16.0 && bFirst) // Drawing connector start point. Line length must be greater than 4 pixels
	{
		ctxDraw.m_DC.Rectangle (ptBegin.x, ptBegin.y - 1, ptBegin.x + 1, ptBegin.y + 2);
	}

	if (distSqr >= 36.0 && bLast) // Drawing connector arrow. Line length must be greater than 6 pixels
	{
		IPOINT pts[3];
		pts [0] = ptEnd;
		pts [1] = ptEnd;
		pts [2] = ptEnd;

		const int d1 = 4, d2 = 2; //6,3

		if (ptBegin.x < ptEnd.x) // left-to-right line
		{
			pts[1].x -= d1;
			pts[1].y -= d2;
			pts[2].x -= d1;
			pts[2].y += d2;
		}
		else if (ptBegin.x > ptEnd.x) // right-to-left line
		{
			pts[1].x += d1;
			pts[1].y -= d2;
			pts[2].x += d1;
			pts[2].y += d2;
		}
		else if (ptBegin.y < ptEnd.y) // top-to-bottom line
		{
			pts[1].x -= d2;
			pts[1].y -= d1;
			pts[2].x += d2;
			pts[2].y -= d1;
		}
		else if (ptBegin.y > ptEnd.y) // bottom-to-top line
		{
			pts[1].x -= d2;
			pts[1].y += d1;
			pts[2].x += d2;
			pts[2].y += d1;
		}

		ctxDraw.m_DC.Polygon (pts, 3);
	}
}

CBCGPGanttDrawContext::CBCGPGanttDrawContext(const CBCGPGanttItem& item, CDC& dc)
    : m_Item(item)
    , m_DC(dc)
    , m_clrFill(globalData.clrBtnFace)
    , m_clrFill2(globalData.clrBtnShadow)
    , m_clrBorder(globalData.clrWindowFrame)
{
}

#endif // !defined (BCGP_EXCLUDE_GRID_CTRL) && !defined (BCGP_EXCLUDE_GANTT)
