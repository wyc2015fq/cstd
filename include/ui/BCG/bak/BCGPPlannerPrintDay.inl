
//
// BCGPPlannerPrintDay.cpp : implementation file
//

#include "BCGPPlannerPrintDay.h"

#ifndef BCGP_EXCLUDE_PLANNER

#include "BCGPPlannerManagerCtrl.h"
#include "BCGPPlannerViewDay.h"

#include <math.h>

/////////////////////////////////////////////////////////////////////////////
// CBCGPPlannerPrintDay

IMPLEMENT_DYNCREATE(CBCGPPlannerPrintDay, CBCGPPlannerPrint)

CBCGPPlannerPrintDay::CBCGPPlannerPrintDay()
	: CBCGPPlannerPrint ()
	, m_TimeDelta        (CBCGPPlannerView::BCGP_PLANNER_TIME_DELTA_60)
{
}

CBCGPPlannerPrintDay::~CBCGPPlannerPrintDay()
{
}

#ifdef _DEBUG
void CBCGPPlannerPrintDay::AssertValid() const
{
	CObject::AssertValid();
}

void CBCGPPlannerPrintDay::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
}
#endif

void CBCGPPlannerPrintDay::DrawAppointment (CDC* pDC, CBCGPAppointment* pApp, CBCGPAppointmentDrawStruct* pDS)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pApp);

	if (pDC == NULL || pDC == NULL || pApp == NULL || pDS == NULL)
	{
		return;
	}

	CRect rect (pDS->GetRect());

	COleDateTime dtStart  (pApp->GetStart());
	COleDateTime dtFinish (pApp->GetFinish());

	CString      strStart  (pApp->m_strStart);
	CString      strFinish (pApp->m_strFinish);

	BOOL bAlternative = pApp->IsAllDay() || pApp->IsMultiDay();
	BOOL bEmpty       = (dtStart == dtFinish) && !bAlternative;

	const int nMinuts  = CBCGPPlannerView::GetTimeDeltaInMinuts (GetTimeDelta());

	BOOL bDrawTime = !bAlternative;

	if (!bAlternative)
	{
		CRect rt (rect);
		rt.r = rt.l + CBCGPPlannerViewDay::BCGP_PLANNER_DURATION_BAR_WIDTH * m_OnePoint.w;

		{
			CBrush br (_RGB (255, 255, 255));
			pDC->FillRect (rt, &br);
		}

		{
			BOOL bNonSingleDay = !CBCGPPlannerView::IsOneDay (dtStart, dtFinish);

			if (bNonSingleDay)
			{
				if (pDS->GetBorder() == CBCGPAppointmentDrawStruct::BCGP_APPOINTMENT_DS_BORDER_START)
				{
					dtFinish  = COleDateTime (dtFinish.GetYear(), dtFinish.GetMonth(), dtFinish.GetDay(),
						0, 0, 0);
				}
				else if (pDS->GetBorder() == CBCGPAppointmentDrawStruct::BCGP_APPOINTMENT_DS_BORDER_FINISH)
				{
					dtStart  = COleDateTime (dtFinish.GetYear(), dtFinish.GetMonth(), dtFinish.GetDay(),
						0, 0, 0);
				}
			}

			double dStart = 
				(dtStart.GetHour() * 60 + dtStart.GetMinute()) / (double)nMinuts;
			double dEnd = CBCGPPlannerView::IsOneDay (dtStart, dtFinish)
				? (dtFinish.GetHour() * 60 + dtFinish.GetMinute()) / (double)nMinuts
				: 24 * 60 / (double)nMinuts;

			int nStart = (int)dStart;
			int nEnd   = (int)ceil(dEnd);

			CBrush br (pApp->GetDurationColor() == CLR_DEFAULT ? m_clrPen : pApp->GetDurationColor());

			if (!bEmpty)
			{
				double dDelta = (double)rt.Height() / (double)(nEnd - nStart);

				CRect rt1 (rt);
				rt1.t    += long((dStart - nStart) * dDelta);
				rt1.b -= long((nEnd - dEnd) * dDelta);

				pDC->FillRect (rt1, &br);
			}
			else
			{
				IPOINT points[3];
				points[0].x = rt.l;
				points[0].y = rt.t;
				points[1].x = rt.r;
				points[1].y = rt.t;
				points[2].x = rt.r;
				points[2].y = rt.t + rt.r - rt.l;

				CRgn rgn;
				rgn.CreatePolygonRgn (points, 3, WINDING);

				pDC->FillRgn (&rgn, &br);
			}

			if (bNonSingleDay)
			{
				dtStart  = pApp->GetStart();
				dtFinish = pApp->GetFinish();

				dStart = 
					(dtStart.GetHour() * 60 + dtStart.GetMinute()) / (double)nMinuts;
				dEnd = CBCGPPlannerView::IsOneDay (dtStart, dtFinish)
					? (dtFinish.GetHour() * 60 + dtFinish.GetMinute()) / (double)nMinuts
					: 24 * 60 / (double)nMinuts;

				nStart = (int)dStart;
				nEnd   = (int)ceil(dEnd);
			}

			bDrawTime = dStart != nStart || dEnd != nEnd;
		}

		CPen penBlack (PS_SOLID, 0, m_clrPen);
		CPen* pOldPen = pDC->SelectObject (&penBlack);

		pDC->MoveTo (rt.r, rt.t);
		pDC->LineTo (rt.r, rt.b);

		pDC->SelectObject (pOldPen);

		rect.l = rt.r + m_OnePoint.w;
	}
	else
	{
		rect.DeflateRect (4 * m_OnePoint.w, 0);
	}

	{
		CBrush br (pApp->GetBackgroundColor() == CLR_DEFAULT ? _RGB (255, 255, 255) : pApp->GetBackgroundColor());
		pDC->FillRect (rect, &br);
	}

	COLORREF clrOld = pDC->SetTextColor (pApp->GetForegroundColor() != CLR_DEFAULT 
		? pApp->GetForegroundColor()
		: m_clrText);

	if (bDrawTime)
	{
		CSize szSpace (pDC->GetTextExtent (_T(" ")));

		CString str;

		if (!strStart.IsEmpty())
		{
			str = strStart;

			if (!strFinish.IsEmpty() && !bEmpty)
			{
				str += _T("-") + strFinish;
			}
		}

		if (!str.IsEmpty())
		{
			CSize sz (pDC->GetTextExtent (str));

			CRect rectText (rect);
			rectText.DeflateRect (4 * m_OnePoint.w, m_OnePoint.h, m_OnePoint.w, 0);

			pDC->DrawText (str, rectText, DT_NOPREFIX);	

			rect.l += sz.w + szSpace.w;
		}
	}

	BOOL bDrawText = !pApp->m_strDescription.IsEmpty();

	CSize szText (0, 0);
	if (bDrawText)
	{
		szText = pDC->GetTextExtent (pApp->m_strDescription);
	}

	if (bDrawText)
	{
		CRect rectText (rect);

		if (bAlternative && szText.w < rectText.Width())
		{
			rectText.l += (rectText.Width() - szText.w) / 2;
		}
		else
		{
			// if time drawed and description not completely in view, then
			// move description down (if possible)
			if (bDrawTime)
			{
				if ((szText.w + 4 * m_OnePoint.w) > rect.Width() && 
					rect.Height() > szText.h * 2)
				{
					rectText.l = pDS->GetRect().l + 
						(CBCGPPlannerViewDay::BCGP_PLANNER_DURATION_BAR_WIDTH + 1) * m_OnePoint.w;
					rectText.OffsetRect (0, szText.h);
				}
			}
		}

		rectText.DeflateRect (4 * m_OnePoint.w, m_OnePoint.h, m_OnePoint.w, 0);

		pDC->DrawText (pApp->m_strDescription, rectText, DT_NOPREFIX);
	}

	if (!bAlternative)
	{
		rect.l = pDS->GetRect().l;
	}

	CPen pen (PS_SOLID, 0, m_clrPen);

	HBRUSH hOldBrush = (HBRUSH)::SelectObject (pDC, ::GetStockObject (NULL_BRUSH));
	CPen* pOldPen = (CPen*)pDC->SelectObject (&pen);

	if (!bAlternative || 
		(bAlternative && pDS->GetBorder() == CBCGPAppointmentDrawStruct::BCGP_APPOINTMENT_DS_BORDER_ALL))
	{
		pDC->Rectangle (rect);
	}
	else if (bAlternative)
	{
		pDC->MoveTo (rect.l , rect.t);
		pDC->LineTo (rect.r, rect.t);

		pDC->MoveTo (rect.l , rect.b - m_OnePoint.h);
		pDC->LineTo (rect.r, rect.b - m_OnePoint.h);

		if (pDS->GetBorder() & CBCGPAppointmentDrawStruct::BCGP_APPOINTMENT_DS_BORDER_START)
		{
			pDC->MoveTo (rect.l, rect.t);
			pDC->LineTo (rect.l, rect.b);
		}

		if (pDS->GetBorder() & CBCGPAppointmentDrawStruct::BCGP_APPOINTMENT_DS_BORDER_FINISH)
		{
			pDC->MoveTo (rect.r - m_OnePoint.w, rect.t);
			pDC->LineTo (rect.r - m_OnePoint.w, rect.b);
		}
	}

	::SelectObject (pDC, hOldBrush);
	pDC->SelectObject (pOldPen);

	pDC->SetTextColor (clrOld);
}

void CBCGPPlannerPrintDay::OnDrawAppointmentsDuration (CDC* pDC)
{
	XBCGPAppointmentArray& arApps = GetQueryedAppointments();

	if (arApps.GetSize() == 0)
	{
		return;
	}

	COleDateTime dtS = GetDateStart();
	COleDateTime dtE = GetDateEnd();

	const int nMinuts  = CBCGPPlannerView::GetTimeDeltaInMinuts (GetTimeDelta());
	const int nCount  = 24 * 60 / nMinuts;
	const int yOffset = 0;

	for (int i = 0; i < arApps.GetSize(); i++)
	{
		CBCGPAppointment* pApp = arApps [i];
		if (pApp == NULL || pApp->IsAllDay() || !pApp->IsMultiDay())
		{
			continue;
		}

		COleDateTime dtStart  = pApp->GetStart();
		COleDateTime dtFinish = pApp->GetFinish();

		dtStart.SetDate (dtStart.GetYear(), dtStart.GetMonth(), dtStart.GetDay());
		dtFinish.SetDate (dtFinish.GetYear(), dtFinish.GetMonth(), dtFinish.GetDay());

		BOOL bStart = FALSE;
		BOOL bEnd   = FALSE;

		if (dtStart < dtS)
		{
			dtStart = dtS;
			bStart  = TRUE;
		}

		if (dtE < dtFinish)
		{
			dtFinish = dtE;
			bEnd     = TRUE;
		}

		COleDateTimeSpan span (dtFinish - dtStart);

		int nStart = (dtStart - dtS).GetDays();
		int nEnd   = min(nStart + span.GetDays() + 1, GetViewDuration());

		CBrush br (pApp->GetDurationColor() == CLR_DEFAULT
			? m_clrPen
			: pApp->GetDurationColor());

		for(int i = nStart; i < nEnd; i++)
		{
			CRect rt (m_ViewRects[i]);

			rt.r  = rt.l + 
				(CBCGPPlannerViewDay::BCGP_PLANNER_DURATION_BAR_WIDTH + 1) * m_OnePoint.w;
			rt.l  -= (i == 0) ? m_OnePoint.w : 0;
			rt.t   -= m_OnePoint.h;
			rt.b = rt.t + nCount * m_nRowHeight;
			rt.DeflateRect (m_OnePoint.w, 0);
			
			rt.OffsetRect (0, -yOffset);

			if (i == (nEnd - 1) && !bEnd)
			{
				dtFinish = pApp->GetFinish();
				const double dDelta = (dtFinish.GetHour() * 60 + dtFinish.GetMinute()) / (double)nMinuts;

				rt.b = rt.t + CBCGPPlannerView::round(dDelta * m_nRowHeight);
			}

			if (i == nStart && !bStart)
			{
				dtStart  = pApp->GetStart();
				const double dDelta = 
					(dtStart.GetHour() * 60 + dtStart.GetMinute()) / (double)nMinuts;

				rt.t += CBCGPPlannerView::round(dDelta * m_nRowHeight);
			}

			rt.IntersectRect (rt, m_ViewRects[i]);

			pDC->FillRect (rt, &br);
		}
	}
}

void CBCGPPlannerPrintDay::GetCaptionFormatStrings (CStringArray& sa)
{
	sa.RemoveAll();

	BOOL bCompact = (GetDrawFlags() & BCGP_PLANNER_DRAW_VIEW_CAPTION_DAY_COMPACT) ==
			BCGP_PLANNER_DRAW_VIEW_CAPTION_DAY_COMPACT;

	if (!bCompact)
	{
		CString strSep (CBCGPPlannerView::GetDateSeparator());

		if (CBCGPPlannerView::IsDateBeforeMonth())
		{
			sa.Add (_T("dddd d MMMM"));
			sa.Add (_T("dddd d MMM"));
			sa.Add (_T("ddd d MMM"));
			sa.Add (_T("d") + strSep + _T("M"));
		}
		else
		{
			sa.Add (_T("dddd MMMM d"));
			sa.Add (_T("dddd MMM d"));
			sa.Add (_T("ddd MMM d"));
			sa.Add (_T("M") + strSep + _T("d"));
		}
	}
	else
	{
		sa.Add (_T("d\ndddd"));
		sa.Add (_T("d\nddd"));
		sa.Add (_T("d"));
	}
}

void CBCGPPlannerPrintDay::AdjustLayout (CDC* /*pDC*/, const CRect& rectClient)
{
	m_nHeaderHeight       = GetViewDuration() > 1 ? 1 : 0;
	m_nHeaderAllDayHeight = 1;

	int nCount = 24 * 60 / CBCGPPlannerView::GetTimeDeltaInMinuts (GetTimeDelta());

	m_rectTimeBar = rectClient;
	m_rectTimeBar.r = m_rectTimeBar.l + m_nRowHeight * 3 + 5 * m_OnePoint.w;
	
	m_rectApps.l = m_rectTimeBar.r;

	{
		// finding allday or multiday events

		CDWordArray arDays;
		arDays.SetSize (GetViewDuration());

		COleDateTime dtS = GetDateStart();

		XBCGPAppointmentArray& arApps = GetQueryedAppointments();

		int i = 0;
		for (i = 0; i < arApps.GetSize(); i++)
		{
			const CBCGPAppointment* pApp = arApps[i];
			if (pApp == NULL)
			{
				continue;
			}

			ASSERT_VALID (pApp);

			if (pApp->IsAllDay() || pApp->IsMultiDay())
			{
				if (arDays.GetSize() > 1)
				{
					COleDateTime dtStart  = pApp->GetStart();
					COleDateTime dtFinish = pApp->GetFinish();

					dtStart = COleDateTime (dtStart.GetYear(), dtStart.GetMonth(), dtStart.GetDay(),
						0, 0, 0);

					if (pApp->IsAllDay())
					{
						dtFinish += COleDateTimeSpan (1, 0, 0, 0);
					}
					else if (pApp->GetFinish().GetHour() != 0 ||
							 pApp->GetFinish().GetMinute() != 0)
					{
						dtFinish = COleDateTime (dtFinish.GetYear(), dtFinish.GetMonth(), dtFinish.GetDay(),
							0, 0, 0);
						dtFinish += COleDateTimeSpan (1, 0, 0, 0);
					}

					if (dtStart < dtS)
					{
						dtStart = dtS;
					}

					COleDateTimeSpan span (dtFinish - dtStart);

					int nStart = (dtStart - dtS).GetDays();
					int nEnd   = nStart + span.GetDays();

					if (nEnd > (int) arDays.GetSize())
					{
						nEnd = (int) arDays.GetSize();
					}

					for (int j = nStart; j < nEnd; j++)
					{
						arDays[j] = arDays[j] + 1;
					}
				}
				else
				{
					arDays[0] = arDays[0] + 1;
				}
			}
		}

		DWORD maxCount = 0;
		for (i = 0; i < arDays.GetSize(); i++)
		{
			if (maxCount < arDays[i])
			{
				maxCount = arDays[i];
			}
		}

		if (maxCount > 0)
		{
			m_nHeaderAllDayHeight = maxCount;
		}
	}

	int nRow = rectClient.Height() / 
		(nCount + m_nHeaderHeight + m_nHeaderAllDayHeight);

	int nOldRowHeight = m_nRowHeight;

	if (nRow > m_nRowHeight)
	{
		m_nRowHeight = nRow;
	}

	m_nHeaderHeight       *= m_nRowHeight;
	m_nHeaderAllDayHeight *= (m_nRowHeight + 2 * m_OnePoint.h);

	m_rectApps.t += m_nHeaderHeight;

	nRow = (m_rectApps.Height() - m_nHeaderAllDayHeight) / nCount;

	if (nRow > nOldRowHeight)
	{
		m_nRowHeight = nRow;
	}

	int delta = m_rectApps.Height() - m_nHeaderAllDayHeight;

	if (delta < 0)
	{
		m_nHeaderAllDayHeight = m_rectApps.Height();
	}
	else
	{
		int nc = (int)(delta / m_nRowHeight);

		if (nc >= nCount)
		{
			m_nHeaderAllDayHeight = m_rectApps.Height() - nCount * m_nRowHeight;
		}
		else
		{
			m_nHeaderAllDayHeight += delta - nc * m_nRowHeight;
		}
	}

	m_rectApps.t += m_nHeaderAllDayHeight;
}

void CBCGPPlannerPrintDay::AdjustRects()
{
	const int nDays = GetViewDuration();
	const int dxColumn = CBCGPPlannerView::round (m_rectApps.Width() / (double)nDays);

	CRect rect (m_rectApps);
	rect.r = rect.l + dxColumn;

	for (int nDay = 0; nDay < nDays; nDay++)
	{
		m_ViewRects.Add (rect);

		rect.OffsetRect (dxColumn, 0);

		if (nDay == (nDays - 2))
		{
			rect.r = m_rectApps.r;
		}
	}
}

void CBCGPPlannerPrintDay::AdjustAppointments()
{
	XBCGPAppointmentArray& arQueryApps = GetQueryedAppointments();

	const int nDays = GetViewDuration();

	if (arQueryApps.GetSize() == 0 || m_ViewRects.GetSize() != nDays)
	{
		return;
	}

	COleDateTime date (m_DateStart);

	for (int i = 0; i < arQueryApps.GetSize(); i++)
	{
		CBCGPAppointment* pApp = arQueryApps[i];
		ASSERT_VALID (pApp);

		pApp->ResetPrint();
	}

	const int nTimeDelta = CBCGPPlannerView::GetTimeDeltaInMinuts (GetTimeDelta());
	const int yOffset = 0;//m_nScrollOffset * m_nRowHeight;

	COleDateTimeSpan spanDay (1, 0, 0, 0);

	XBCGPAppointmentArray arAllDays;

	int nDay = 0;
	const int c_Space = CBCGPPlannerViewDay::BCGP_PLANNER_APPOINTMENT_SPACE * m_OnePoint.w;

	for (nDay = 0; nDay < nDays; nDay ++)
	{	
		XBCGPAppointmentArray arByDate;

		int i;

		CRect rectFill (m_ViewRects [nDay]);
		rectFill.t -= yOffset;

		for (i = 0; i < arQueryApps.GetSize(); i++)
		{
			CBCGPAppointment* pApp = arQueryApps[i];
			ASSERT_VALID (pApp);

			if (CBCGPPlannerView::IsAppointmentInDate (*pApp, date))
			{
				CRect rtApp (rectFill);

				// trimming top and bottom edge, starting from TimeDelta and Appointment interval
				rtApp.t = rectFill.t + m_nRowHeight * 
					int((pApp->GetStart().GetHour() * 60 + 
						 pApp->GetStart().GetMinute()) / nTimeDelta) - m_OnePoint.h;

				if (pApp->IsAllDay() || pApp->IsMultiDay())
				{
					BOOL bAdd = TRUE;

					for (int i = 0; i < arAllDays.GetSize(); i++)
					{
						if (arAllDays[i] == pApp)
						{
							bAdd = FALSE;
							break;
						}
					}

					if (bAdd)
					{
						arAllDays.Add (pApp);
					}
				}
				else
				{
					rtApp.r -= 2 * c_Space;

					if (pApp->GetStart() != pApp->GetFinish())
					{
						COleDateTime dtAF (pApp->GetFinish());
						int minutes = dtAF.GetHour() * 60 + dtAF.GetMinute();

						if (!CBCGPPlannerView::IsOneDay (pApp->GetStart(), dtAF))
						{
							BOOL bStartDay   = CBCGPPlannerView::IsOneDay (date, pApp->GetStart());
							BOOL bFinishNULL = dtAF.GetHour() == 0 && 
											   dtAF.GetMinute() == 0;

							if (bStartDay)
							{
								minutes = 24 * 60;
							}

							if (bFinishNULL)
							{
								if (!bStartDay)
								{
									if (pApp->GetDSPrint().GetCount() == 1)
									{
										pApp->GetDSPrint()[0].m_date2 = date;
										continue;
									}
								}
							}
							else
							{
								if (!bStartDay)
								{
									rtApp.t = rectFill.t - m_OnePoint.h;
								}
							}
						}

						rtApp.b = rectFill.t + m_nRowHeight * 
							(long)ceil(minutes / (double)nTimeDelta) - m_OnePoint.h;
					}
					else
					{
						rtApp.b = rtApp.t + m_nRowHeight;
					}

					pApp->SetRectPrint (rtApp, date);
					arByDate.Add (pApp);
				}
			}
		}

		// resort appointments in the view, if count of collection is great than 1
		if (arByDate.GetSize() > 1)
		{
			XBCGPAppointmentArray* ar = new XBCGPAppointmentArray;

			// array, that contains columns
			CArray<XBCGPAppointmentArray*, XBCGPAppointmentArray*> arColumns;
			arColumns.Add (ar);

			// initialize first column
			ar->Copy (arByDate);

			while (ar != NULL)
			{
				CBCGPAppointment* pApp1 = ar->GetAt (0);

				XBCGPAppointmentArray* arNew = NULL;

				i = 1;

				// remove appointmets, that have collisions in rects, from previous column 
				// to the next column
				while (i < ar->GetSize())
				{
					CBCGPAppointment* pApp2 = ar->GetAt (i);

					CRect rtInter;
					if (rtInter.IntersectRect (pApp1->GetRectPrint (date), pApp2->GetRectPrint (date)))
					{
						if (arNew == NULL)
						{
							// add a new column
							arNew = new XBCGPAppointmentArray;
							arColumns.Add (arNew);
						}

						arNew->Add (pApp2);
						ar->RemoveAt (i);
					}
					else
					{
						pApp1 = pApp2;
						i++;
					}
				}

				ar = arNew;
			}

			int nCount = (int) arColumns.GetSize();

			// reinitialize drawing rects, if found great than 1 columns
			if (nCount > 1)
			{
				int nWidth = rectFill.Width() / nCount;
				int nL = rectFill.l;

				// left border of appointments, based on column order
				for (i = 0; i < nCount; i++)
				{
					ar = arColumns[i];

					for (int j = 0; j < ar->GetSize(); j++)
					{
						CBCGPAppointment* pApp = ar->GetAt (j);

						CRect rtApp (pApp->GetRectPrint (date));
						rtApp.l = nL;

						pApp->SetRectPrint (rtApp, date);
					}

					nL += nWidth;
				}

				// correcting right border of appointments
				for (i = 0; i < nCount; i++)
				{
					ar = arColumns[i];

					for (int j = 0; j < ar->GetSize(); j++)
					{
						CBCGPAppointment* pApp = ar->GetAt (j);
						CRect rtApp (pApp->GetRectPrint (date));

						for (int k = i + 1; k < nCount; k++)
						{
							XBCGPAppointmentArray* arNext = arColumns[k];

							for (int m = 0; m < arNext->GetSize(); m++)
							{
								CBCGPAppointment* pAppNext = arNext->GetAt (m);

								CRect rtInter;
								if (rtInter.IntersectRect (rtApp, pAppNext->GetRectPrint (date)))
								{
									rtApp.r = rectFill.l + nWidth * k - c_Space;
									pApp->SetRectPrint (rtApp, date);
									break;
								}
							}
						}
					}
				}
			}

			// clean up columns array
			for (i = 0; i < nCount; i++)
			{
				delete arColumns[i];
			}

			arColumns.RemoveAll();
		}
		
		date += spanDay;
	}

	// adjust "all day" or "multi day" appointments
	if (arAllDays.GetSize() > 0)
	{
		const int c_Count = (int) arAllDays.GetSize();

		CRect rectFill (m_ViewRects [0]);
		rectFill.t    = m_rectApps.t - 
				(m_nHeaderHeight + m_nHeaderAllDayHeight) + m_OnePoint.h;
		rectFill.b = rectFill.t + m_nRowHeight;

		int i = 0;

		for (i = 0; i < c_Count; i++)
		{
			CBCGPAppointment* pApp = arAllDays[i];

			date = m_DateStart;

			for (nDay = 0; nDay < nDays; nDay ++)
			{
				rectFill.l  = m_ViewRects [nDay].l;
				rectFill.r = m_ViewRects [nDay].r;

				if (CBCGPPlannerView::IsAppointmentInDate (*pApp, date))
				{
					pApp->SetRectPrint (rectFill, date);
				}

				date += spanDay;
			}
		}

		for (i = 1; i < c_Count; i++)
		{
			CBCGPAppointment* pApp1 = arAllDays[i];

			CRect rtApp1;
			if (pApp1->GetDSPrint().IsEmpty())
			{
				rtApp1 = pApp1->GetRectPrint();
			}
			else
			{
				rtApp1 = pApp1->GetDSPrint().GetByIndex (0)->GetRect();
			}

			for (int j = 0; j < i; j++)
			{
				CBCGPAppointment* pApp2 = arAllDays[j];

				CRect rtApp2;
				if (pApp2->GetDSPrint().IsEmpty())
				{
					rtApp2 = pApp2->GetRectPrint();
				}
				else
				{
					rtApp2 = pApp2->GetDSPrint().GetByIndex (0)->GetRect();
				}

				CRect rtInter;
				if (rtInter.IntersectRect (rtApp1, rtApp2))
				{
					rtApp1.t    = rtApp2.t;
					rtApp1.b = rtApp2.b;
					rtApp1.OffsetRect (0, m_nRowHeight + 2 * m_OnePoint.h);
					
					if (pApp1->GetDSPrint().IsEmpty())
					{
						pApp1->SetRectPrint (rtApp1);
					}
					else
					{
						pApp1->GetDSPrint().GetByIndex (0)->SetRect (rtApp1);
					}

					j = 0;
				}
			}
		}
	}

	date = m_DateStart;

	for (nDay = 0; nDay < nDays; nDay ++)
	{
		CheckVisibleAppointments (date, m_rectApps, FALSE);

		date += spanDay;
	}
}

void CBCGPPlannerPrintDay::CalculateDates (const COleDateTime& date)
{
	m_DateStart.SetDate (date.GetYear(), date.GetMonth(), date.GetDay());
	m_DateEnd   = m_DateStart + COleDateTimeSpan (0, 23, 59, 59);
}

void CBCGPPlannerPrintDay::OnPaint (CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT_VALID (pDC);
	ASSERT (pInfo != NULL);

	CBCGPPlannerPrint::OnPaint (pDC, pInfo);

	int nOldBk = pDC->SetBkMode (TRANSPARENT);
	CFont* pOldFont = pDC->SelectObject (&m_Font);

	CRect rectFrame (m_rectApps);

	OnDrawClient (pDC);

	OnDrawAppointmentsDuration (pDC);

	if (m_nHeaderAllDayHeight != 0)
	{
		CRect rtHeader (m_rectApps);
		rtHeader.t    -= m_nHeaderAllDayHeight;
		rtHeader.b  = rtHeader.t + m_nHeaderAllDayHeight;

		OnDrawHeaderAllDay (pDC, rtHeader);

		rectFrame.t = rtHeader.t;
	}

	OnDrawAppointments (pDC, m_rectApps);

	if (!m_rectTimeBar.IsRectEmpty())
	{
		OnDrawTimeBar (pDC, m_rectTimeBar);

		rectFrame.l = m_rectTimeBar.l;
	}

	if (m_nHeaderHeight != 0)
	{
		CRect rtHeader (m_rectApps);
		rtHeader.t    -= (m_nHeaderHeight + m_nHeaderAllDayHeight);
		rtHeader.b = rtHeader.t + m_nHeaderHeight;

		OnDrawHeader (pDC, rtHeader);

		rectFrame.t = rtHeader.t;
	}

	pDC->SelectObject (pOldFont);
	pDC->SetBkMode (nOldBk);

	{
		rectFrame.InflateRect (m_OnePoint.w, m_OnePoint.h);

		HBRUSH hOldBrush = (HBRUSH)::SelectObject (pDC, ::GetStockObject (NULL_BRUSH));
		CPen* pOldPen = (CPen*)pDC->SelectObject (&m_penBlack);

		pDC->Rectangle (rectFrame);

		::SelectObject (pDC, hOldBrush);
		pDC->SelectObject (pOldPen);
	}
}

void CBCGPPlannerPrintDay::OnDrawClient (CDC* pDC)
{
	ASSERT_VALID (pDC);

	CRect rectFill (m_rectApps);

	const int yOffset = 0;//m_nScrollOffset * m_nRowHeight;

	const int nDays = GetViewDuration();

	rectFill.OffsetRect (0, -yOffset);

	const int nMinuts = CBCGPPlannerView::GetTimeDeltaInMinuts (GetTimeDelta());
	const int nCount = 60 / nMinuts;

	CPen* pOldPen = pDC->SelectObject (&m_penBlack);

	const int iStart = 0;//m_nScrollOffset;
	const int iEnd   = min (m_rectApps.Height() / m_nRowHeight, nCount * 24);//min (m_nScrollOffset + rect.Height() / m_nRowHeight, nCount * 24);

	COleDateTime dtStart (GetDateStart());
	int nDay = 0;

	for (nDay = 0; nDay < nDays; nDay++)
	{
		rectFill = m_ViewRects [nDay];

		rectFill.l   += (CBCGPPlannerViewDay::BCGP_PLANNER_DURATION_BAR_WIDTH + 1) * m_OnePoint.w;
		rectFill.b = rectFill.t + m_nRowHeight - m_OnePoint.h;

		for (int iStep = iStart; iStep < iEnd; iStep++)
		{
			pDC->MoveTo (rectFill.l, rectFill.b);
			pDC->LineTo (rectFill.r, rectFill.b);

			rectFill.OffsetRect (0, m_nRowHeight);
		}

		dtStart += COleDateTimeSpan (1, 0, 0, 0);
	}

	rectFill = m_rectApps;

	for (nDay = 0; nDay < nDays; nDay++)
	{
		CRect rectDurBar (m_ViewRects [nDay]);
		rectDurBar.r = rectDurBar.l + CBCGPPlannerViewDay::BCGP_PLANNER_DURATION_BAR_WIDTH * m_OnePoint.w;

		// Draw duration bar (at left):
		pDC->FillRect (rectDurBar, &globalData.brWindow);

		if (nDay > 0)
		{
			pDC->MoveTo (rectDurBar.l, rectDurBar.t);
			pDC->LineTo (rectDurBar.l, rectDurBar.b);
		}

		pDC->MoveTo (rectDurBar.r, rectDurBar.t);
		pDC->LineTo (rectDurBar.r, rectDurBar.b);
	}

	pDC->SelectObject (pOldPen);
}

void CBCGPPlannerPrintDay::OnDrawHeader (CDC* pDC, const CRect& rectHeader)
{
	ASSERT_VALID (pDC);

	const int nDays = GetViewDuration();

	DrawHeader (pDC, rectHeader, m_ViewRects [0].Width());

	CRect rectDayCaption (rectHeader);

	COleDateTime day (GetDateStart());

	for (int nDay = 0; nDay < nDays; nDay++)
	{
		rectDayCaption.l = m_ViewRects [nDay].l;
		rectDayCaption.r = m_ViewRects [nDay].r;

		DrawCaption (pDC, rectDayCaption, day, TRUE, TRUE, m_brGray);

		day += COleDateTimeSpan (1, 0, 0, 0);
	}
}

void CBCGPPlannerPrintDay::OnDrawHeaderAllDay (CDC* pDC, const CRect& rectHeader)
{
	ASSERT_VALID (pDC);

	const int nDays = GetViewDuration();

	pDC->FillRect (rectHeader, &m_brGray);

	CPen* pOldPen = pDC->SelectObject (&m_penBlack);

	if (nDays > 1)
	{
		for (int nDay = 1; nDay < nDays; nDay++)
		{
			pDC->MoveTo (m_ViewRects [nDay].l, rectHeader.t);
			pDC->LineTo (m_ViewRects [nDay].l, rectHeader.b - m_OnePoint.h);
		}
	}

	pDC->MoveTo (rectHeader.l, rectHeader.b - m_OnePoint.h);
	pDC->LineTo (rectHeader.r, rectHeader.b - m_OnePoint.h);

	pDC->SelectObject (pOldPen);
}

void CBCGPPlannerPrintDay::OnDrawTimeBar (CDC* pDC, const CRect& rectBar)
{
	ASSERT_VALID (pDC);

	BOOL b24Hours = CBCGPPlannerView::Is24Hours();

	CString strAM;
	CString strPM;

	if (!b24Hours)
	{
		strAM = CBCGPPlannerView::GetTimeDesignator (TRUE);
		strAM.MakeLower();
		strPM = CBCGPPlannerView::GetTimeDesignator (FALSE);
		strPM.MakeLower();
	}

	pDC->FillRect (rectBar, &m_brGray);

	const int nHeaderHeight = m_nHeaderHeight + m_nHeaderAllDayHeight;

	int y = rectBar.t + nHeaderHeight - m_OnePoint.h;

	CPen* pOldPen = pDC->SelectObject (&m_penBlack);

	const long nCount = 60 / CBCGPPlannerView::GetTimeDeltaInMinuts (GetTimeDelta());

	UINT nFormat = DT_SINGLELINE | DT_RIGHT;

	if (nCount == 1)
	{
		nFormat |= DT_VCENTER;
	}


	LOGFONT lf;
	m_Font.GetLogFont (&lf);

	lf.lfHeight *= 2;

	CFont fontBold;
	fontBold.CreateFontIndirect (&lf);

	int bDrawFirstAM_PM = 0;

	for (int i = 0; i < 24; i++)
	{
		CRect rectHour  = rectBar;
		rectHour.t    = y;
		rectHour.b = y + m_nRowHeight * nCount;
		rectHour.l   += 5 * m_OnePoint.w;
		rectHour.r  -= 5 * m_OnePoint.h;

		if(rectHour.b < nHeaderHeight)
		{
			y = rectHour.b;
			continue;
		}

		if (nCount > 2)
		{
			long nd = y + m_nRowHeight;

			for (int j = 0; j < nCount - 1; j++)
			{
				if (nd >= nHeaderHeight)
				{
					pDC->MoveTo (rectHour.r - 18 * m_OnePoint.w, nd);
					pDC->LineTo (rectHour.r, nd);
				}

				nd += m_nRowHeight;
			}
		}

		if (rectHour.b >= nHeaderHeight)
		{
			y += m_nRowHeight * nCount;

			pDC->MoveTo (rectHour.l , y);
			pDC->LineTo (rectHour.r, y);
		}

		if (rectHour.t >= nHeaderHeight || rectHour.b > nHeaderHeight)
		{
			if (rectHour.t >= nHeaderHeight - m_OnePoint.h)
			{
				bDrawFirstAM_PM++;
			}

			CString str (_T("00"));

			int nHour = i;

			BOOL bAM = nHour < 12; 

			if (!b24Hours)
			{
				if (nHour == 0 || nHour == 12)
				{
					nHour = 12;
				}
				else if (nHour > 12)
				{
					nHour -= 12;
				}
			}

			if (nCount == 1)
			{
				str.Format (_T("%2d:00"), nHour);

				if (!b24Hours)
				{
					if (nHour == 12)
					{
						str.Format (_T("12 %s"), bAM ? strAM : strPM);
					}
				}

				pDC->DrawText (str, rectHour, nFormat);
			}
			else
			{
				y = rectHour.b;

				rectHour.b = rectHour.t + m_nRowHeight;

				if (!b24Hours)
				{
					if (nHour == 12 || bDrawFirstAM_PM == 1)
					{
						str = bAM ? strAM : strPM;
					}
				}

				CRect rectMin (rectHour);
				rectMin.l   = rectMin.r - 18 * m_OnePoint.w;

				pDC->DrawText (str, rectMin, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

				pDC->SelectObject (&fontBold);

				rectHour.b = y;
				rectHour.r -= 18 * m_OnePoint.w;

				str.Format (_T("%d"), nHour);
				pDC->DrawText (str, rectHour, nFormat);

				pDC->SelectObject (&m_Font);
			}
		}

		y = rectHour.b;
	}

	CRect rt (rectBar);
	rt.b = rt.t + nHeaderHeight;

	pDC->FillRect (rt, &m_brGray);

	pDC->MoveTo (rectBar.l, rt.b - m_OnePoint.h);
	pDC->LineTo (rectBar.r - 5 * m_OnePoint.w, rt.b - m_OnePoint.h);

	pDC->MoveTo (rectBar.r - m_OnePoint.w, rectBar.t);
	pDC->LineTo (rectBar.r - m_OnePoint.w, rectBar.b);

	pDC->SelectObject (pOldPen);
}

void CBCGPPlannerPrintDay::OnDrawPageHeader (CDC* pDC)
{
	if (m_rectPageHeader.IsRectEmpty())
	{
		return;
	}

	{
		CBrush* pOldBrush = (CBrush*)pDC->SelectObject (&m_brGray);
		CPen* pOldPen = (CPen*)pDC->SelectObject (&m_penBlack);

		pDC->Rectangle (m_rectPageHeader);

		pDC->SelectObject (pOldBrush);
		pDC->SelectObject (pOldPen);
	}


	SYSTEMTIME st;
	m_Date.GetAsSystemTime (st);

	CString strFmtDate;
	if (CBCGPPlannerView::IsDateBeforeMonth())
	{
		strFmtDate = _T("d MMMM yyyy");
	}
	else
	{
		strFmtDate = _T("MMMM d yyyy");
	}

	TCHAR sz[100];
	::GetDateFormat (LOCALE_USER_DEFAULT, 0, &st, strFmtDate, sz, 100);

	CString str1 (sz);

	::GetDateFormat (LOCALE_USER_DEFAULT, 0, &st, _T("dddd"), sz, 100);

	CString str2 (sz);

	int nOldBk = pDC->SetBkMode (TRANSPARENT);
	CFont* pOldFont = pDC->SelectObject (&m_FontHeader);

	CRect rt (m_rectPageHeader);
	rt.DeflateRect (m_OnePoint.w * 4, m_OnePoint.h);

	pDC->DrawText (str1, rt, DT_LEFT);

	TEXTMETRIC tm;
	pDC->GetTextMetrics (&tm);

	rt.t += tm.tmHeight;

	LOGFONT lf;
	m_FontHeader.GetLogFont (&lf);

	lf.lfHeight *= 2;
	lf.lfHeight /= 3;
	lf.lfWeight = FW_NORMAL;

	CFont font;
	font.CreateFontIndirect (&lf);

	pDC->SelectObject (&font);

	pDC->DrawText (str2, rt, DT_LEFT);

	pDC->SelectObject (pOldFont);
	pDC->SetBkMode (nOldBk);
}

void CBCGPPlannerPrintDay::CheckVisibleAppointments(const COleDateTime& date, const CRect& rect, 
	BOOL bFullVisible)
{
	XBCGPAppointmentArray& arApps = GetQueryedAppointments();

	if (arApps.GetSize() == 0)
	{
		return;
	}

	BOOL bSelect = date != COleDateTime();

	for (int i = 0; i < arApps.GetSize(); i++)
	{
		CBCGPAppointment* pApp = arApps [i];

		if (bSelect)
		{
			if (!CBCGPPlannerView::IsAppointmentInDate (*pApp, date))
			{
				continue;
			}
		}

		CRect rt (rect);
		if (pApp->IsAllDay() || pApp->IsMultiDay())
		{
			rt.t    -= m_nHeaderAllDayHeight;
			rt.b = rt.t + m_nHeaderAllDayHeight;
		}

		CRect rtDraw (pApp->GetRectPrint (date));

		CRect rtInter;
		rtInter.IntersectRect (rtDraw, rt);

		pApp->SetVisiblePrint ((!bFullVisible && rtInter.Height() >= 2) || 
			(bFullVisible && rtInter.Height() == rtDraw.Height() && 
			 rtInter.b < rt.b), date);
	}
}

#endif // BCGP_EXCLUDE_PLANNER
