
//
// BCGPPlannerClockIcons.cpp : implementation file
//

#include "BCGPPlannerClockIcons.h"

#ifndef BCGP_EXCLUDE_PLANNER

#ifndef _BCGPCALENDAR_STANDALONE
	#include "bcgprores.h"
#else
	#include "resource.h"
#endif



/////////////////////////////////////////////////////////////////////////////
// CBCGPPlannerClockIcons

IMPLEMENT_DYNCREATE(CBCGPPlannerClockIcons, CObject)

CBCGPPlannerClockIcons::CBCGPPlannerClockIcons()
	:	m_szSize       (0, 0)
	,	m_bInitialized (FALSE)
{
}

CBCGPPlannerClockIcons::~CBCGPPlannerClockIcons()
{
}

BOOL CBCGPPlannerClockIcons::Initialize()
{
	if (IsInitialized())
	{
		return TRUE;
	}

	CImageList	ilClock;
	CImageList	ilMinuts;

	CSize szSize (0, 0);

	{
		CBCGPLocalResource locaRes;

		CBitmap bmp;
		if (!bmp.LoadBitmap (IDB_BCGBARRES_PLANNER_APP_CLOCK))
		{
			return FALSE;
		}

		BITMAP bm;
		bmp.GetBitmap (&bm);

		szSize.w = bm.bmWidth / 48;
		szSize.h = bm.bmHeight;

		ilClock.Create (szSize.w, szSize.h, ILC_COLOR, 48, 0);
		ilClock.Add (&bmp, (CBitmap*)NULL);
	}

	if (ilClock.IsValid())
	{
		CBCGPLocalResource locaRes;

		CBitmap bmp;
		if (!bmp.LoadBitmap (IDB_BCGBARRES_PLANNER_APP_CLOCK_M))
		{
			return FALSE;
		}

		BITMAP bm;
		bmp.GetBitmap (&bm);

		ASSERT (szSize.w == bm.bmWidth / 24);
		ASSERT (szSize.h == bm.bmHeight);

		ilMinuts.Create (szSize.w, szSize.h, ILC_COLOR, 24, 0);
		ilMinuts.Add (&bmp, (CBitmap*)NULL);
	}

	if (ilClock.GetSafeHandle() == NULL || ilMinuts.GetSafeHandle() == NULL)
	{
		return FALSE;
	}

	CBitmap bmp;

	CDC* hDC = ::GetDC (NULL);
	CDC* pDC = CDC::FromHandle (hDC);

	CDC dc;
	if (dc.CreateCompatibleDC (pDC))
	{
		if (bmp.CreateCompatibleBitmap (pDC, szSize.w * 24 * 12, szSize.h))
		{
			CBitmap* pOld = dc.SelectObject (&bmp);

			for (int nHour = 0; nHour < 24; nHour++)
			{
				BOOL bPM = nHour >= 12;

				for (int nMinuts = 0; nMinuts < 12; nMinuts++)
				{
					CPoint pt ((nHour * 12 + nMinuts) * szSize.w, 0);

					ilClock.Draw (&dc, nHour * 2 + (nMinuts >= 5 ? 1 : 0), 
						pt , ILD_IMAGE);

					ilMinuts.DrawIndirect
						(
							&dc,
							nMinuts + (bPM ? 12 : 0),
							pt,
							szSize,
							CPoint (0, 0),
							ILD_IMAGE | ILD_ROP,
							bPM ? SRCPAINT : SRCAND
						);
				}
			}

		    dc.SelectObject (pOld);
		}
	}

	::ReleaseDC (NULL, hDC);

	if (bmp.IsValid())
	{
		m_szSize = szSize;

		m_ilClockIcons.Create (m_szSize.w, m_szSize.h, ILC_COLOR | ILC_MASK, 
			24 * 12, 0);
		m_ilClockIcons.Add (&bmp, _RGB (255, 255, 255));
		m_bInitialized = TRUE;
	}

	return m_bInitialized;
}

#ifdef _DEBUG
void CBCGPPlannerClockIcons::AssertValid() const
{
	CObject::AssertValid();
}

void CBCGPPlannerClockIcons::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
}
#endif

void CBCGPPlannerClockIcons::DrawIcon(CDC* pDC, const CPoint& point, 
									   const COleDateTime& time)
{
	Initialize();

	ASSERT_VALID (pDC);

	if (m_ilClockIcons.GetSafeHandle() == NULL || pDC == NULL || 
		pDC == NULL)
	{
		return;
	}

	DrawIcon (pDC, point, time.GetHour(), time.GetMinute());
}

CSize CBCGPPlannerClockIcons::GetIconSize()
{
	Initialize();
	return m_szSize;
}

void CBCGPPlannerClockIcons::DrawIcon(CDC* pDC, const CPoint& point, 
									  int nHour, int nMin)
{
	Initialize();

	int nImage = nHour * 12 + nMin / 5;

	if (0 <= nImage && nImage < m_ilClockIcons.GetImageCount())
	{
		m_ilClockIcons.Draw (pDC, nImage, point, ILD_NORMAL);
	}
}

#endif // BCGP_EXCLUDE_PLANNER
