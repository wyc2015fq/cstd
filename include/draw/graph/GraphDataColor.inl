//GraphDataColor.cpp - Version 3.0 (Brian Convery, May, 2001)

// GraphDataColor.cpp : implementation file
// Download by http://www.NewXing.com

#include "stdafx.h"
#include "testdraw.h"	//replace with your app
#include "Graph.h"
#include "GraphDataColor.h"

/////////////////////////////////////////////////////////////////////////////
// CGraphDataColor

CGraphDataColor::CGraphDataColor(int group, COLORREF color)
{
	dataGroup = group;
	groupColor = color;
}

CGraphDataColor::~CGraphDataColor()
{
}


/////////////////////////////////////////////////////////////////////////////
// CGraphDataColor message handlers

COLORREF CGraphDataColor::GetColor()
{
	return groupColor;
}
