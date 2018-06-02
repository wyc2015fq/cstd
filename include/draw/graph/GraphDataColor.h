//Download by http://www.NewXing.com
//GraphDataColor.h - Version 3.0 (Brian Convery, May, 2001)

#if !defined(AFX_GRAPHDATACOLOR_H__3CBDC4C5_494F_11D5_AD40_006008198224__INCLUDED_)
#define AFX_GRAPHDATACOLOR_H__3CBDC4C5_494F_11D5_AD40_006008198224__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GraphDataColor.h : header file
//

#include "Graph.h"

/////////////////////////////////////////////////////////////////////////////
// CGraphDataColor window

class CGraphDataColor
{
	int dataGroup;	//just an id indicator to help searching
// Construction
public:
	CGraphDataColor(int group, COLOR color);

// Attributes
public:
	COLOR groupColor;

// Operations
public:
	COLOR GetColor();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphDataColor)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraphDataColor();

};

// #include "GraphDataColor.inl"

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHDATACOLOR_H__3CBDC4C5_494F_11D5_AD40_006008198224__INCLUDED_)
