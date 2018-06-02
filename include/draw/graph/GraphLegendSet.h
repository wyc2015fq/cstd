//GraphLegendSet.h - Version 3.0 (Brian Convery, May, 2001)
// Download by http://www.NewXing.com
#if !defined(AFX_GRAPHLEGENDSET_H__3CBDC4C4_494F_11D5_AD40_006008198224__INCLUDED_)
#define AFX_GRAPHLEGENDSET_H__3CBDC4C4_494F_11D5_AD40_006008198224__INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// CGraphLegendSet window

class CGraphLegendSet
{
// Construction
public:
	CGraphLegendSet();

// Attributes
public:
	int legendId;	//only used for searching through a list of this object

private :
	char* legendLabel;

// Operations
public:
	void SetLegendLabel(int id, char* legendText);
	char* GetLegendLabel();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphLegendSet)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraphLegendSet();

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHLEGENDSET_H__3CBDC4C4_494F_11D5_AD40_006008198224__INCLUDED_)
