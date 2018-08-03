
//
// BCGPRibbonMainPanel.h: interface for the CBCGPRibbonMainPanel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPRIBBONMAINPANEL_H__4FC2195A_9736_4B67_88CF_A5778C816CA0__INCLUDED_)
#define AFX_BCGPRIBBONMAINPANEL_H__4FC2195A_9736_4B67_88CF_A5778C816CA0__INCLUDED_

#include "BCGPRibbonPanel.h"
#include "BCGPRibbonButton.h"

#ifndef BCGP_EXCLUDE_RIBBON

class CBCGPRibbonMainPanelButton;

class BCGCBPRODLLEXPORT CBCGPRibbonRecentFilesList : public CBCGPRibbonButtonsGroup
{
	DECLARE_DYNCREATE(CBCGPRibbonRecentFilesList)

public:
	CBCGPRibbonRecentFilesList(LPCTSTR lpszLabel = NULL)
	{
		SetText (lpszLabel == NULL ? _T("") : lpszLabel);
	}

	void FillList();

protected:
	virtual void OnAfterChangeRect (CDC* pDC);
	virtual CSize GetRegularSize (CDC* pDC);
	virtual void OnDraw (CDC* pDC);
	virtual BOOL OnMenuKey (UINT nUpperChar);
};

class BCGCBPRODLLEXPORT CBCGPRibbonMainPanel : public CBCGPRibbonPanel  
{
	friend class CBCGPRibbonMainButton;
	friend class CBCGPRibbonCollector;

	DECLARE_DYNCREATE (CBCGPRibbonMainPanel)

// Construction
public:
	CBCGPRibbonMainPanel();
	virtual ~CBCGPRibbonMainPanel();

// Operations
public:
	virtual void Add (CBCGPBaseRibbonElement* pElem);
	void AddToBottom (CBCGPRibbonMainPanelButton* pElem);

	void AddRecentFilesList (LPCTSTR lpszLabel, int nWidth = 300);
	void AddToRight (CBCGPBaseRibbonElement* pElem, int nWidth = 300);

protected:
	int GetMenuElements() const;
	void DrawMainButton (CDC* pDC, CWnd* pWnd);

// Overrides
protected:

	virtual void RecalcWidths (CDC* pDC, int nHeight);
	virtual void Repos (CDC* pDC, const CRect& rect);
	virtual void DoPaint (CDC* pDC);

	virtual BOOL IsFixedSize() const
	{
		return TRUE;
	}

	virtual void CopyFrom (CBCGPRibbonPanel& src);
	virtual BOOL GetPreferedMenuLocation (CRect& rect);

	virtual BOOL IsMainPanel() const
	{
		return TRUE;
	}

	virtual void OnDrawMenuBorder (CDC* pDC, CBCGPRibbonPanelMenuBar* pMenuBar);
	virtual CBCGPBaseRibbonElement* MouseButtonDown (CPoint point);

// Attributes:
public:
	CRect GetCommandsFrame() const;

protected:
	int						m_nBottomElementsNum;
	CRect					m_rectMenuElements;
	int						m_nTopMargin;
	CBCGPRibbonMainButton*	m_pMainButton;
	CBCGPBaseRibbonElement* m_pElemOnRight;
	int						m_nRightPaneWidth;
};

class BCGCBPRODLLEXPORT CBCGPRibbonMainPanelButton : public CBCGPRibbonButton
{
	DECLARE_DYNCREATE(CBCGPRibbonMainPanelButton)

// Construction
public:
	CBCGPRibbonMainPanelButton();

	CBCGPRibbonMainPanelButton (
		UINT	nID, 
		LPCTSTR lpszText, 
		int		nSmallImageIndex);

	CBCGPRibbonMainPanelButton (
		UINT	nID, 
		LPCTSTR lpszText, 
		CBitmap*	hIcon);

	virtual ~CBCGPRibbonMainPanelButton();

// Overrides
protected:
	virtual COLORREF OnFillBackground (CDC* pDC);
	virtual void OnDrawBorder (CDC* pDC);
};

#endif // BCGP_EXCLUDE_RIBBON

#endif // !defined(AFX_BCGPRIBBONMAINPANEL_H__4FC2195A_9736_4B67_88CF_A5778C816CA0__INCLUDED_)