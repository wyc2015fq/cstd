//Download by http://www.NewXing.com
//GraphLegend.h - Version 3.0 (Brian Convery, May, 2001)

#if !defined(AFX_GRAPHLEGEND_H__99A8A552_4029_40CC_839A_D8210C46CD8B__INCLUDED_)
#define AFX_GRAPHLEGEND_H__99A8A552_4029_40CC_839A_D8210C46CD8B__INCLUDED_

#include "GraphLegendSet.h"

/////////////////////////////////////////////////////////////////////////////
// CGraphLegend window

class CGraphLegend
{
// Construction
public:
	CGraphLegend();

// Attributes
public:


private:
	int legendLength;
	void *legendLabels;

// Operations
public:
	void SetLegendText(int datagroup, char* label);
	char* GetLegendText(int datagroup);
	int GetLegendLength();
	void Remove(int dataGroup);
	void RemoveAll();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphLegend)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraphLegend();

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHLEGEND_H__99A8A552_4029_40CC_839A_D8210C46CD8B__INCLUDED_)
