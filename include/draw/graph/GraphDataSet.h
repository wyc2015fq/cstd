//Download by http://www.NewXing.com
//GraphDataSet.h - Version 3.0 (Brian Convery, May, 2001)

#if !defined(AFX_GRAPHDATASET_H__3CBDC4C1_494F_11D5_AD40_006008198224__INCLUDED_)
#define AFX_GRAPHDATASET_H__3CBDC4C1_494F_11D5_AD40_006008198224__INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// CGraphDataSet window

struct CGraphDataSet
{
// Construction
	CGraphDataSet();

// Attributes
public:

	int xPosition;
	int yValue;

// Operations
public:
	void SetXPosition(int x);
	void SetYValue(int y);
	int GetXData();
	int GetYData();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphDataSet)
	//}}AFX_VIRTUAL

// Implementation
public:
	~CGraphDataSet();

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHDATASET_H__3CBDC4C1_494F_11D5_AD40_006008198224__INCLUDED_)
