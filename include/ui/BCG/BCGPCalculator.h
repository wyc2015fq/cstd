// This is a part of the BCGControlBar Library
// Copyright (C) 1998-2009 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.

#if !defined(AFX_BCGPCALCULATOR_H__9B759C19_6B1C_48C4_A884_C0DEDD60DD77__INCLUDED_)
#define AFX_BCGPCALCULATOR_H__9B759C19_6B1C_48C4_A884_C0DEDD60DD77__INCLUDED_
// BCGPCalculator.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBCGPCalculator window

enum CalculatorCommands
{
  idCommandNone,
  idCommandClear,
  idCommandReset,
  idCommandMemClear,
  idCommandMemRead,
  idCommandMemAdd,
  idCommandAdd,
  idCommandSub,
  idCommandMult,
  idCommandDiv,
  idCommandSign,
  idCommandDot,
  idCommandSqrt,
  idCommandPercent,
  idCommandResult,
  idCommandRev,
  idCommandBackspace,
  idCommandUser = 100
};

struct CCalculatorButton
{
  button_t btn;
  char m_strText[20];
  BOOL m_bText;
  BOOL m_bImage;
  void init(short nDigit = 0)
  {
    _snprintf(m_strText, 20, _T("%d"), nDigit);
    m_nDigit = nDigit;
    m_uiCmd = idCommandNone;
    m_bText = TRUE;
    m_bImage = FALSE;
    m_nKbdAccel = _T('0') + nDigit;
    m_nKbdAccel2 = 0;
    m_pParent = NULL;
    m_bIsVitKey = FALSE;
    m_bIsCtrl = FALSE;
    m_bIsUserCommand = FALSE;
  }
  
  void init(LPCTSTR lpszName, CalculatorCommands uiCMD, int nKbdAccel,
    int nKbdAccel2 = 0, BOOL bIsVitKey = FALSE, BOOL bIsCtrl = FALSE,
    BOOL bIsUserCommand = FALSE)
  {
    ASSERT(lpszName != NULL);
    
    strcpy(m_strText ,lpszName);
    m_nDigit = -1;
    m_uiCmd = uiCMD;
    m_bText = TRUE;
    m_bImage = FALSE;
    m_nKbdAccel = nKbdAccel;
    m_nKbdAccel2 = nKbdAccel2;
    m_pParent = NULL;
    m_bIsVitKey = bIsVitKey;
    m_bIsCtrl = bIsCtrl;
    m_bIsUserCommand = bIsUserCommand;
  }
  
  void OnDraw (CDC* pDC, const CRect& rect, img_t* pImages,
    BOOL bHorz = TRUE, BOOL bCustomizeMode = FALSE,
    BOOL bHighlight = FALSE,
    BOOL bDrawBorder = TRUE,
    BOOL bGrayDisabledButtons = TRUE);
  
  short								m_nDigit;
  CalculatorCommands	m_uiCmd;
  int									m_nKbdAccel;
  int									m_nKbdAccel2;
  CBCGPCalculator*		m_pParent;
  BOOL								m_bIsVitKey;
  BOOL								m_bIsCtrl;
  BOOL								m_bIsUserCommand;
  unsigned int m_nStyle;
  int SetRect(const CRect& rc) {
    return 0;
  }
};

struct CBCGPCalculator
{
  ctrl_t c[1];
// Construction
	CBCGPCalculator();
	CBCGPCalculator(double dblValue, UINT nID, CBCGPEdit* pParentEdit);

protected:
	CBCGPCalculator (CBCGPCalculator& src, UINT uiCommandID);

// Operations:
public:
	void RemoveAdditionalCommands();

// Attributes
public:

	void SetValue (double dblValue);
	double GetValue() const
	{
		return m_dblValue;
	}

	void SetError (BOOL bIsError = TRUE)
	{
		m_bIsError = bIsError;
		UpdateDisplay();
	}

	double GetSecondValue() const
	{
		return m_dblSecondValue;
	}

	double GetMemValue() const
	{
		return m_dblMemValue;
	}

	UINT GetCommandID() const
	{
		return m_nCommandID;
	}

	void SetCommandID (UINT nCommandID)
	{
		m_nCommandID = nCommandID;
	}

protected:
	double						m_dblValue;
	double						m_dblSecondValue;
	double						m_dblMemValue;
	char						m_strBuffer[32];
	UINT						m_uiLastCmd;
	UINT						m_nCommandID;
	BOOL						m_bIsEnabled;
	BOOL						m_bInternal;	// Used in customization
	int							m_nVertMargin;
	int							m_nHorzMargin;
	CRect						m_rectDisplay;
	BOOL						m_bIsClearBuffer;
	BOOL						m_bIsError;
  BOOL						m_bSeqResult;
  BOOL						m_bLocked;
  BOOL						m_bInUpdateShadow;
	int							m_nRows;
	int							m_nColumns;
  CCalculatorButton m_Buttons[256];
  int m_nButton;

  // Overrides

	virtual void OnUserCommand (UINT uiCmd	/* Start from idCommandUser */);

	virtual BOOL OnDrawButton (CDC* pDC, CRect rect,
		CCalculatorButton* pButton, BCGBUTTON_STATE state, 
		int cmd /* CBCGPCalculator::CalculatorCommands */);
	virtual BOOL OnDrawDisplay (CDC* pDC, CRect rect, const CString& strText, BOOL bMem);

	virtual BOOL OnSendCommand (const CCalculatorButton* pButton);
	virtual void AdjustLocations(const CRect& rcClient);
	virtual CSize CalcSize (BOOL bVertDock);

	virtual BOOL AllowChangeTextLabels() const
	{
		return FALSE;
	}

	virtual BOOL AllowShowOnList() const
	{	
		return TRUE;	
	}

	virtual BOOL AllowShowOnControlMenu() const
	{	
		return FALSE;
	}

	virtual void Rebuild();
	virtual void PushDigit (short nDigit);
	virtual BOOL OnProcessKey (int nKey, BOOL bIsVirtKey);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPCalculator)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

#if (defined _BCGSUITE_) || (defined _BCGSUITE_INC_)
	virtual void OnFillBackground (CDC* pDC);
#endif

// Implementation
public:
	//{{AFX_MSG(CBCGPCalculator)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNcPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg LRESULT OnMouseLeave(WPARAM,LPARAM);
	DECLARE_MESSAGE_MAP()

	void CommonInit();

	void UpdateDisplay();
	void UpdateBuffer();
	void CalcResult();
};

#endif // !defined(AFX_BCGPCALCULATOR_H__9B759C19_6B1C_48C4_A884_C0DEDD60DD77__INCLUDED_)
