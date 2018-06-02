
// ImagePaintArea.cpp : implementation file
//

#include "bcgprores.h"
#include "ImagePaintArea.h"

#include "BCGPImageEditDlg.h"

#include "BCGGlobals.h"

/////////////////////////////////////////////////////////////////////////////
// CImagePaintArea

CImagePaintArea::CImagePaintArea(CBCGPImageEditDlg* pParentDlg)
{
	ASSERT (pParentDlg != NULL);

	m_pParentDlg = pParentDlg;

	m_sizeImage.w = 0;
	m_sizeImage.h = 0;

	m_pBitmap = NULL;
	m_rgbColor = _RGB (0, 0, 0);	// Black

	m_rectParentPreviewArea.SetRectEmpty();
	m_memDC.CreateCompatibleDC (NULL);

	m_Mode = IMAGE_EDIT_MODE_PEN;
	m_rectDraw.SetRectEmpty();

	m_penDraw.CreatePen (PS_SOLID, 1, m_rgbColor);
}

CImagePaintArea::~CImagePaintArea()
{
	::DestroyCursor (m_hcurPen);
	::DestroyCursor (m_hcurFill);
	::DestroyCursor (m_hcurLine);
	::DestroyCursor (m_hcurRect);
	::DestroyCursor (m_hcurEllipse);
	::DestroyCursor (m_hcurColor);
}

BEGIN_MESSAGE_MAP(CImagePaintArea, CButton)
	//{{AFX_MSG_MAP(CImagePaintArea)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CANCELMODE()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImagePaintArea message handlers

void CImagePaintArea::DrawItem (LPDRAWITEMSTRUCT lpDIS)
{
	CDC* pDC = CDC::FromHandle (lpDIS->hDC);
	ASSERT_VALID (pDC);

	CRect rectClient = lpDIS->rcItem;

	pDC->FillRect (&rectClient, &globalData.brBtnFace);
	rectClient.InflateRect (-1, -1);

	CRect rectDraw = rectClient;

	rectDraw.r = rectDraw.l + m_sizeCell.w * m_sizeImage.w;
	rectDraw.b = rectDraw.t + m_sizeCell.h * m_sizeImage.h;

	rectClient = rectDraw;
	rectClient.InflateRect (1, 1);

	pDC->Draw3dRect (rectDraw, globalData.clrBtnDkShadow, globalData.clrBtnHilite);

	//-----------
	// Draw grid:
	//-----------
	CPen penGrid (PS_SOLID, 1, globalData.clrBtnShadow);
	CPen* pOldPen = (CPen*) pDC->SelectObject (&penGrid);

	int x = 0;
	int y = 0;

	for (x = rectDraw.l + m_sizeCell.w; 
		x <= rectDraw.r - m_sizeCell.w; x += m_sizeCell.w)
	{
		pDC->MoveTo (x, rectDraw.t + 1);
		pDC->LineTo (x, rectDraw.b - 1);
	}

	for (y = rectDraw.t + m_sizeCell.h; 
		y <= rectDraw.b - m_sizeCell.h; y += m_sizeCell.h)
	{
		pDC->MoveTo (rectDraw.l + 1, y);
		pDC->LineTo (rectDraw.r - 1, y);
	}

	pDC->SelectObject (pOldPen);

	//-------------
	// Draw bitmap:
	//-------------
	if (m_pBitmap == NULL)
	{
		return;
	}

	CBitmap* pOldBitmap = m_memDC.SelectObject (m_pBitmap);

	for (x = 0; x < m_sizeImage.w; x ++)
	{
		for (y = 0; y < m_sizeImage.h; y ++)
		{
			COLORREF rgbPixel = 
				CBCGPToolBarImages::MapFromSysColor (m_memDC.GetPixel (x, y), FALSE);
			if (rgbPixel != (COLORREF) -1)
			{
				CRect rect (
					CPoint (rectDraw.l + x * m_sizeCell.w, 
							rectDraw.t + y * m_sizeCell.h),
					m_sizeCell);
				rect.InflateRect (-1, -1);

				pDC->FillSolidRect (rect, rgbPixel);
			}
		}
	}

	m_memDC.SelectObject (pOldBitmap);
}
//****
BOOL CImagePaintArea::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}
//****
void CImagePaintArea::OnMouseMove(UINT nFlags, CPoint point) 
{
	if ((nFlags & MK_LBUTTON) == 0)
	{
		return;
	}

	CRect rectClient;	// Client area rectangle
	GetClientRect (&rectClient);

	point.x = min (max (point.x, rectClient.l), rectClient.r);
	point.y = min (max (point.y, rectClient.t), rectClient.b);

	if (m_Mode == IMAGE_EDIT_MODE_PEN)
	{
		DrawPixel (point);
		return;
	}

	if (m_Mode != IMAGE_EDIT_MODE_LINE &&
		m_Mode != IMAGE_EDIT_MODE_RECT &&
		m_Mode != IMAGE_EDIT_MODE_ELLIPSE)
	{
		return;
	}

	CRect rectDraw = rectClient;

	rectDraw.r = rectDraw.l + m_sizeCell.w * m_sizeImage.w;
	rectDraw.b = rectDraw.t + m_sizeCell.h * m_sizeImage.h;

	rectDraw.DeflateRect (1, 1);

	if (m_rectDraw == rectDraw)
	{
		return;
	}

	CRgn rgnClip;
	rgnClip.CreateRectRgnIndirect (&rectDraw);

	CClientDC dc (this);
	dc.SelectObject (&rgnClip);

	CPen* pOldPen = (CPen*) dc.SelectObject (&m_penStretch);
	CBrush* pOldBrush = (CBrush*) dc.SelectStockObject (NULL_BRUSH);
	int nOldROP = dc.SetROP2 (R2_NOT);

	switch (m_Mode)
	{
	case IMAGE_EDIT_MODE_LINE:
		{		
			//-----------
			// Erase old:
			//-----------
			CRect rectScreen = m_rectDraw;
			BitmapToClient (rectScreen);

			if (m_rectDraw.Width() != 0 ||
				m_rectDraw.Height() != 0)
			{
				dc.MoveTo (rectScreen.l, rectScreen.t);
				dc.LineTo (rectScreen.r, rectScreen.b);
			}

			ScreenToBitmap (point);
			m_rectDraw.r = point.x;
			m_rectDraw.b = point.y;

			//----------
			// Draw new:
			//----------
			rectScreen = m_rectDraw;
			BitmapToClient (rectScreen);

			if (m_rectDraw.Width() != 0 ||
				m_rectDraw.Height() != 0)
			{
				dc.MoveTo (rectScreen.l, rectScreen.t);
				dc.LineTo (rectScreen.r, rectScreen.b);
			}
		}
		break;

	case IMAGE_EDIT_MODE_RECT:
		{		
			//-----------
			// Erase old:
			//-----------
			CRect rectScreen = m_rectDraw;
			BitmapToClient (rectScreen);

			if (m_rectDraw.Width() != 0 ||
				m_rectDraw.Height() != 0)
			{
				dc.Rectangle (rectScreen);
			}

			ScreenToBitmap (point);
			m_rectDraw.r = point.x;
			m_rectDraw.b = point.y;

			//----------
			// Draw new:
			//----------
			rectScreen = m_rectDraw;
			BitmapToClient (rectScreen);

			if (m_rectDraw.Width() != 0 ||
				m_rectDraw.Height() != 0)
			{
				dc.Rectangle (rectScreen);
			}
		}
		break;

	case IMAGE_EDIT_MODE_ELLIPSE:
		{		
			//-----------
			// Erase old:
			//-----------
			CRect rectScreen = m_rectDraw;
			BitmapToClient (rectScreen);

			if (m_rectDraw.Width() != 0 ||
				m_rectDraw.Height() != 0)
			{
				dc.Ellipse (rectScreen);
			}

			ScreenToBitmap (point);
			m_rectDraw.r = point.x;
			m_rectDraw.b = point.y;

			//----------
			// Draw new:
			//----------
			rectScreen = m_rectDraw;
			BitmapToClient (rectScreen);

			if (m_rectDraw.Width() != 0 ||
				m_rectDraw.Height() != 0)
			{
				dc.Ellipse (rectScreen);
			}
		}
		break;
	}

	dc.SetROP2 (nOldROP);
	dc.SelectObject (pOldBrush);
	dc.SelectObject (pOldPen);
	dc.SelectClipRgn (NULL);
}
//****
void CImagePaintArea::OnLButtonDown(UINT /*nFlags*/, CPoint point) 
{
	if (m_pBitmap == NULL)
	{
		return;
	}

	CRect rectDraw;
	GetClientRect (rectDraw);

	rectDraw.r = rectDraw.l + m_sizeCell.w * m_sizeImage.w;
	rectDraw.b = rectDraw.t + m_sizeCell.h * m_sizeImage.h;

	rectDraw.DeflateRect (1, 1);

	if (!rectDraw.PtInRect (point))
	{
		return;
	}

	switch (m_Mode)
	{
	case IMAGE_EDIT_MODE_PEN:
		DrawPixel (point);
		break;

	case IMAGE_EDIT_MODE_LINE:
	case IMAGE_EDIT_MODE_RECT:
	case IMAGE_EDIT_MODE_ELLIPSE:
		ScreenToBitmap (point);
		m_rectDraw = CRect (point, CSize (0, 0));
		break;
	}

	SetCapture();
}
//****
void CImagePaintArea::OnLButtonUp(UINT /*nFlags*/, CPoint point) 
{
	if (GetCapture() == this)
	{
		ReleaseCapture();
	}

	if (m_Mode == IMAGE_EDIT_MODE_RECT || m_Mode == IMAGE_EDIT_MODE_ELLIPSE)
	{
		if (m_rectDraw.Height() != 0)
		{
			if (m_rectDraw.t < m_rectDraw.b)
			{
				m_rectDraw.b ++;
			}
			else
			{
				m_rectDraw.t ++;
			}
		}

		if (m_rectDraw.Width() != 0)
		{
			if (m_rectDraw.l < m_rectDraw.r)
			{
				m_rectDraw.r ++;
			}
			else
			{
				m_rectDraw.l ++;
			}
		}
	}

	switch (m_Mode)
	{
	case IMAGE_EDIT_MODE_PEN:
		DrawPixel (point);
		break;

	case IMAGE_EDIT_MODE_LINE:
		if (m_rectDraw.Height() == 0 && m_rectDraw.Width() == 0)
		{
			DrawPixel (point);
		}
		else
		{
			CBitmap* pOldBitmap = m_memDC.SelectObject (m_pBitmap);
			CPen* pOldPen = (CPen*) m_memDC.SelectObject (&m_penDraw);

			m_memDC.MoveTo (m_rectDraw.l, m_rectDraw.t);
			m_memDC.LineTo (m_rectDraw.r, m_rectDraw.b);

			DrawPixel (point);

			m_memDC.SelectObject (pOldBitmap);
			m_memDC.SelectObject (pOldPen);

			Invalidate();
			UpdateWindow();

			GetParent()->InvalidateRect (m_rectParentPreviewArea);
		}
		break;

	case IMAGE_EDIT_MODE_RECT:
		if (m_rectDraw.Height() == 0 && m_rectDraw.Width() == 0)
		{
			DrawPixel (point);
		}
		else
		{
			CBitmap* pOldBitmap = m_memDC.SelectObject (m_pBitmap);
			CPen* pOldPen = (CPen*) m_memDC.SelectObject (&m_penDraw);
			CBrush* pOldBrush = (CBrush*) m_memDC.SelectStockObject (NULL_BRUSH);

			m_memDC.Rectangle (m_rectDraw);

			m_memDC.SelectObject (pOldBitmap);
			m_memDC.SelectObject (pOldPen);
			m_memDC.SelectObject (pOldBrush);

			Invalidate();
			UpdateWindow();

			GetParent()->InvalidateRect (m_rectParentPreviewArea);
		}
		break;

	case IMAGE_EDIT_MODE_ELLIPSE:
		if (m_rectDraw.Height() == 0 && m_rectDraw.Width() == 0)
		{
			DrawPixel (point);
		}
		else
		{
			CBitmap* pOldBitmap = m_memDC.SelectObject (m_pBitmap);
			CPen* pOldPen = (CPen*) m_memDC.SelectObject (&m_penDraw);
			CBrush* pOldBrush = (CBrush*) m_memDC.SelectStockObject (NULL_BRUSH);

			m_memDC.Ellipse (m_rectDraw);

			m_memDC.SelectObject (pOldBitmap);
			m_memDC.SelectObject (pOldPen);
			m_memDC.SelectObject (pOldBrush);

			Invalidate();
			UpdateWindow();

			GetParent()->InvalidateRect (m_rectParentPreviewArea);
		}
		break;

	case IMAGE_EDIT_MODE_FILL:
		ScreenToBitmap (point);
		FloodFil (point);
		break;

	case IMAGE_EDIT_MODE_COLOR:
		{
			ScreenToBitmap (point);

			CBitmap* pOldBitmap = m_memDC.SelectObject (m_pBitmap);
			COLORREF color = m_memDC.GetPixel (point);
			m_memDC.SelectObject (pOldBitmap);

			m_pParentDlg->OnPickColor (color);
		}
		break;
	}

	m_rectDraw.SetRectEmpty();
}
//****
void CImagePaintArea::OnCancelMode() 
{
	if (GetCapture() == this)
	{
		ReleaseCapture();
	}
}
//****DrawPixel (point);
void CImagePaintArea::DrawPixel (IPOINT point)
{
	CRect rectClient;	// Client area rectangle
	GetClientRect (&rectClient);

	rectClient.InflateRect (-1, -1);

	CPoint ptBmp = point;
	ScreenToBitmap (ptBmp);

	CRect rect (
		CPoint (rectClient.l + ptBmp.x * m_sizeCell.w, 
				rectClient.t + ptBmp.y * m_sizeCell.h),
		m_sizeCell);
	rect.InflateRect (-1, -1);

	CClientDC dc (this);
	dc.FillSolidRect (rect, m_rgbColor);

	//---------------
	// Update bitmap:
	//---------------
	CBitmap* pOldBitmap = m_memDC.SelectObject (m_pBitmap);
	m_memDC.SetPixel (ptBmp.x, ptBmp.y, CBCGPToolBarImages::MapToSysColor (m_rgbColor));
	m_memDC.SelectObject (pOldBitmap);

	GetParent()->InvalidateRect (m_rectParentPreviewArea);
}
//****
void CImagePaintArea::SetBitmap (CBitmap* pBitmap)
{
	m_pBitmap = pBitmap;
	m_sizeCell = CSize (0, 0);

	if (m_pBitmap == NULL)
	{
		m_sizeImage.w = 0;
		m_sizeImage.h = 0;
		return;
	}

	BITMAP bmp;
	m_pBitmap->GetBitmap (&bmp);

	m_sizeImage.w = bmp.bmWidth;
	m_sizeImage.h = bmp.bmHeight;

	CRect rectClient;
	GetClientRect (rectClient);
	rectClient.DeflateRect (1, 1);

	m_sizeCell = CSize (
		rectClient.Width() / m_sizeImage.w,
		rectClient.Height() / m_sizeImage.h);

	if (m_penStretch.IsValid())
	{
		m_penStretch.DeleteObject();
	}

	m_penStretch.CreatePen (PS_SOLID, min (m_sizeCell.w, m_sizeCell.h),
							globalData.clrBtnText);
}
//****
BOOL CImagePaintArea::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CPoint ptCursor;
	::GetCursorPos (&ptCursor);
	ScreenToClient (&ptCursor);

	CRect rectDraw;
	GetClientRect (rectDraw);

	rectDraw.r = rectDraw.l + m_sizeCell.w * m_sizeImage.w;
	rectDraw.b = rectDraw.t + m_sizeCell.h * m_sizeImage.h;

	if (rectDraw.PtInRect (ptCursor))
	{
		switch (m_Mode)
		{
		case IMAGE_EDIT_MODE_PEN:
			::SetCursor (m_hcurPen);
			return TRUE;

		case IMAGE_EDIT_MODE_FILL:
			::SetCursor (m_hcurFill);
			return TRUE;

		case IMAGE_EDIT_MODE_LINE:
			::SetCursor (m_hcurLine);
			return TRUE;

		case IMAGE_EDIT_MODE_RECT:
			::SetCursor (m_hcurRect);
			return TRUE;

		case IMAGE_EDIT_MODE_ELLIPSE:
			::SetCursor (m_hcurEllipse);
			return TRUE;

		case IMAGE_EDIT_MODE_COLOR:
			::SetCursor (m_hcurColor);
			return TRUE;
		}
	}
	
	return CButton::OnSetCursor(pWnd, nHitTest, message);
}
//****
void CImagePaintArea::PreSubclassWindow() 
{
	CBCGPLocalResource locaRes;
	
	m_hcurPen = AfxGetApp()->LoadCursor (IDC_BCGBARRES_PEN);
	m_hcurFill = AfxGetApp()->LoadCursor (IDC_BCGBARRES_FILL);
	m_hcurLine = AfxGetApp()->LoadCursor (IDC_BCGBARRES_LINE);
	m_hcurRect = AfxGetApp()->LoadCursor (IDC_BCGBARRES_RECT);
	m_hcurEllipse = AfxGetApp()->LoadCursor (IDC_BCGBARRES_ELLIPSE);
	m_hcurColor = AfxGetApp()->LoadCursor (IDC_BCGBARRES_COLOR);

	CButton::PreSubclassWindow();
}
//****
void CImagePaintArea::ScreenToBitmap (CPoint& point)
{
	int x = (point.x - 1) / m_sizeCell.w;
	int y = (point.y - 1) / m_sizeCell.h;

	point.x = max (min (x, m_sizeImage.w - 1), 0);
	point.y = max (min (y, m_sizeImage.h - 1), 0);
}
//****
void CImagePaintArea::BitmapToClient (CRect& rect)
{
	rect.l = rect.l * m_sizeCell.w + 1;
	rect.t = rect.t * m_sizeCell.h + 1;
	rect.r = rect.r * m_sizeCell.w + 1;
	rect.b = rect.b * m_sizeCell.h + 1;

	rect.OffsetRect (m_sizeCell.w / 2, m_sizeCell.h / 2);
}
//****
void CImagePaintArea::FloodFil (const CPoint& point)
{
	ASSERT (m_pBitmap != NULL);

	CBitmap* pOldBitmap = m_memDC.SelectObject (m_pBitmap);

	CBrush br (m_rgbColor);
	CBrush* pBrOld = (CBrush*) m_memDC.SelectObject (&br);

	m_memDC.ExtFloodFill (point.x, point.y, 
		m_memDC.GetPixel (point), FLOODFILLSURFACE);

	m_memDC.SelectObject (pOldBitmap);
	m_memDC.SelectObject (pBrOld);

	Invalidate();
	UpdateWindow();

	GetParent()->InvalidateRect (m_rectParentPreviewArea);
}
//****
void CImagePaintArea::SetColor (COLORREF color)
{
	m_rgbColor = color;

	if (m_penDraw.IsValid())
	{
		m_penDraw.DeleteObject();
	}

	m_penDraw.CreatePen (PS_SOLID, 1, m_rgbColor);
}
