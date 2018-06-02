
//
// BCGPRibbonHyperlink.h: interface for the CBCGPRibbonHyperlink class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPRIBBONHYPERLINK_H__1EBA0C0E_889A_4A5C_A1E4_DE25C08771BA__INCLUDED_)
#define AFX_BCGPRIBBONHYPERLINK_H__1EBA0C0E_889A_4A5C_A1E4_DE25C08771BA__INCLUDED_

#include "BCGPRibbonButton.h"

#ifndef BCGP_EXCLUDE_RIBBON

class BCGCBPRODLLEXPORT CBCGPRibbonHyperlink : public CBCGPRibbonButton  
{
	DECLARE_DYNCREATE(CBCGPRibbonHyperlink)

// Construction:
public:
	CBCGPRibbonHyperlink();
	CBCGPRibbonHyperlink (
		UINT	nID, 
		LPCTSTR lpszText,
		LPCTSTR lpszLink);

	virtual ~CBCGPRibbonHyperlink();

// Operations:
public:
	BOOL OpenLink();

// Overrides
public:
	virtual CSize GetRegularSize (CDC* pDC);
	virtual CSize GetCompactSize (CDC* pDC);
	virtual void OnDraw (CDC* pDC);
	virtual BOOL OnDrawMenuImage (CDC* /*pDC*/, CRect /*rect*/)	{	return FALSE;	}
	virtual void CopyFrom (const CBCGPBaseRibbonElement& src);
	virtual CString GetToolTipText() const
	{
		return GetLink();
	}

	virtual void OnMouseMove (CPoint point);

	virtual void OnSetIcon();
	virtual BOOL IsDrawTooltipImage() const	{	return FALSE;	}
	
	LPCTSTR GetLink() const
	{
		return m_strLink;
	}
	void SetLink (LPCTSTR lpszLink);

// Attributes:
protected:
	CString	m_strLink;
};

#endif // BCGP_EXCLUDE_RIBBON

#endif // !defined(AFX_BCGPRIBBONHYPERLINK_H__1EBA0C0E_889A_4A5C_A1E4_DE25C08771BA__INCLUDED_)
