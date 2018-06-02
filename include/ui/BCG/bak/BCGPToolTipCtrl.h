//****
// COPYRIGHT NOTES
// ---------------
// This is a part of the BCGControlBar Library
// Copyright (C) 1998-2009 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//****

#if !defined(AFX_BCGPTOOLTIPCTRL_H__5A4EF6E8_E8B6_4161_B4C0_45B99DBCB725__INCLUDED_)
#define AFX_BCGPTOOLTIPCTRL_H__5A4EF6E8_E8B6_4161_B4C0_45B99DBCB725__INCLUDED_
// BCGPToolTipCtrl.h : header file
//

//#include "BCGCBPro.h"

class CBCGPToolBar;
class CBCGPToolbarButton;

class CBCGPRibbonButton;

/////////////////////////////////////////////////////////////////////////////
// CBCGPToolTipParams

/////////////////////////////////////////////////////////////////////////////
// CBCGPToolTipCtrl window

class BCGCBPRODLLEXPORT CBCGPToolTipCtrl : public CToolTipCtrl
{
	DECLARE_DYNCREATE(CBCGPToolTipCtrl)

// Construction
public:
	CBCGPToolTipCtrl(CBCGPToolTipParams* pParams = NULL);

// Attributes
public:
	void SetFixedWidth (int nWidthRegular, int nWidthLargeImage)
	{
		m_nFixedWidthRegular = nWidthRegular;
		m_nFixedWidthWithImage = nWidthLargeImage;
	}

protected:
	CBCGPToolBar*		m_pToolBar;
	CBCGPToolBarImages* m_pToolBarImages;
	CBCGPToolbarButton* m_pHotButton;
	CBCGPRibbonButton*	m_pRibbonButton;
	int					m_nRibbonImageType;
	CString				m_strDesrciption;
	CSize				m_sizeImage;
	CPoint				m_ptMargin;
	CPoint				m_ptLocation;
	int					m_nFixedWidthRegular;
	int					m_nFixedWidthWithImage;

	CBCGPToolTipParams	m_Params;

// Operations
public:
	void SetParams (CBCGPToolTipParams* pParams);	// NULL - default params
	const CBCGPToolTipParams& GetParams() const
	{
		return m_Params;
	}

	void SetLocation (CPoint pt);
	void SetHotRibbonButton (CBCGPRibbonButton* pRibbonButton);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPToolTipCtrl)
	//}}AFX_VIRTUAL

	virtual void SetDescription (const CString strDesrciption);

	virtual CSize GetIconSize();
	virtual void OnFillBackground (CDC* pDC, CRect rect, COLORREF& clrText, COLORREF& clrLine);

	virtual CString GetLabel();

	virtual void OnDrawBorder (CDC* pDC, CRect rect, COLORREF clrLine);
	virtual BOOL OnDrawIcon (CDC* pDC, CRect rectImage);
	virtual CSize OnDrawLabel (CDC* pDC, CRect rect, BOOL bCalcOnly);
	virtual CSize OnDrawDescription (CDC* pDC, CRect rect, BOOL bCalcOnly);
	virtual void OnDrawSeparator (CDC* pDC, int x1, int x2, int y);

// Implementation
public:
	virtual ~CBCGPToolTipCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBCGPToolTipCtrl)
	afx_msg void OnPaint(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	afx_msg void OnShow(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPop(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

	void GetHotButton();
	int GetFixedWidth();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGPTOOLTIPCTRL_H__5A4EF6E8_E8B6_4161_B4C0_45B99DBCB725__INCLUDED_)
