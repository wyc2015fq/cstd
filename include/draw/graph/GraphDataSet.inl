//GraphDataSet.h - Version 3.0 (Brian Convery, May, 2001)

// GraphDataSet.cpp : implementation file
// Download by http://www.NewXing.com

#include "GraphDataSet.h"


/////////////////////////////////////////////////////////////////////////////
// CGraphDataSet

CGraphDataSet::CGraphDataSet()
{
	xPosition = 0;
	yValue = 0;
}

CGraphDataSet::~CGraphDataSet()
{
}

/////////////////////////////////////////////////////////////////////////////
// CGraphDataSet message handlers

void CGraphDataSet::SetXPosition(int x)
{
	xPosition = x;
}

void CGraphDataSet::SetYValue(int y)
{
	yValue = y;
}

int CGraphDataSet::GetXData()
{
	return xPosition;
}

int CGraphDataSet::GetYData()
{
	return yValue;
}
