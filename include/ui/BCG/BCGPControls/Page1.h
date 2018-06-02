
#if !defined(AFX_PAGE1_H__42746B5E_E6CC_48E5_BA21_103576DE34BE__INCLUDED_)
#define AFX_PAGE1_H__42746B5E_E6CC_48E5_BA21_103576DE34BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage1 dialog
struct CPage1
{
  ctrl_t c[1];
	DECLARE_DYNCREATE(CPage1)

// Construction
public:
	CPage1();
	~CPage1();
  
  CGroupBox m_gbox1, m_gbox2, m_gbox3, m_gbox4;
  CStatic m_text1,
    m_text2,
    m_text3,
    m_text4,
    m_text5,
    m_text6,
    m_wndBorderLabel;

	CBCGPButton	m_btnMultiLine;
  CComboBox	m_wndBorder,
  		m_iImage,
  		m_iBorderStyle,
  		m_iCursor,
		m_nImageLocation;

  typedef CBCGPButton CBCGPMenuButton;
	CCheckBox	m_btnCheck;
	CButton	m_wndXPButtons, m_btnSetTooltip;
	BOOL	m_bXPButtons;
	CBCGPButton	m_Button;
	CEdit	m_strToolTip;
	int		m_iFrameRate;
	CBCGPMenuButton	m_btnMenu;
	CCheckBox	m_bMenuStayPressed,
		m_bRightArrow,
  	m_bMenuDefaultClick,
  	m_bCheck;
  
	CRadioBox	m_btnRadio;

// Implementation
	// Generated message map functions
	//{{AFX_MSG(CPage1)
	afx_msg void OnXpButtons();
	virtual BOOL OnInit();
	afx_msg void OnSetTooltip();
	afx_msg void OnButton();
	afx_msg void OnRightArrow();
	afx_msg void OnButtonMenu();
	afx_msg void OnPressedOnMenu();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnItem1();
	afx_msg void OnItem2();
	afx_msg void OnDialogAbout();
	afx_msg void OnResetButton();
	afx_msg void OnMenuDefaultClick();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CMenu		m_menu;

  void OnGetMinMaxInfo(MINMAXINFO* mminfo) {
    mminfo->ptMinTrackSize.x = m_sz.w;
    mminfo->ptMinTrackSize.y = m_sz.h;
    return ;
  }
  ISIZE m_sz;
  ctrl_t* sub[30];
  int nsub;

  void OnPaint(CDC* pDC) {
    int i;
    FillRect(pDC, &c->rc, myGetSysColor(COLOR_3DFACE));
    for (i=2; i<nsub; ++i) {
      sub[i]->redraw = TRUE;
      sub[i]->ctrlproc(c, sub[i], c->hwnd, WM_PAINT, (WPARAM)pDC->im, 0);
      break;
    }
    return ;
  }
};

#include "BCGPControls_rc.inl"

/////////////////////////////////////////////////////////////////////////////
// CPage1 property page

CPage1::CPage1()
{
  memset(c, 0, sizeof(ctrl_t));
	m_bXPButtons = TRUE;
	m_bCheck.SetCheck(TRUE);
  m_iImage.SetCurSel(2);
#if 0
	m_iBorderStyle = 0;
	m_iCursor = 0;
	m_bMenuStayPressed = FALSE;
	m_bRightArrow = FALSE;
	m_nImageLocation = 0;
  m_bMenuDefaultClick = FALSE;
#endif
}

CPage1::~CPage1()
{
}

BEGIN_MESSAGE_MAP(CPage1, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(CPage1)
  ON_WM_CTRLINIT()
  ON_WM_GETMINMAXINFO()
  ON_WM_PAINT()
  ON_BN_CLICKED(IDC_XP_BUTTONS, OnXpButtons)
	ON_BN_CLICKED(IDC_SET_TOOLTIP, OnSetTooltip)
	ON_BN_CLICKED(IDC_BUTTON, OnButton)
	ON_BN_CLICKED(IDC_RIGHT_ARROW, OnRightArrow)
	ON_BN_CLICKED(IDC_BUTTON_MENU, OnButtonMenu)
	ON_BN_CLICKED(IDC_PRESSED_ON_MENU, OnPressedOnMenu)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_ITEM_1, OnItem1)
	ON_COMMAND(ID_ITEM_2, OnItem2)
	ON_COMMAND(ID_DIALOG_ABOUT, OnDialogAbout)
	ON_CBN_SELCHANGE(IDC_IMAGE, OnResetButton)
	ON_CBN_SELCHANGE(IDC_BORDER, OnResetButton)
	ON_CBN_SELCHANGE(IDC_IMAGE, OnResetButton)
	ON_CBN_SELCHANGE(IDC_BORDER, OnResetButton)
	ON_CBN_SELCHANGE(IDC_IMAGE_LOCATION, OnResetButton)
	ON_BN_CLICKED(IDC_MENU_DEFAULT_CLICK, OnMenuDefaultClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage1 message handlers

void CPage1::OnXpButtons() 
{
	EnableWindow(m_wndBorder.c, !m_bXPButtons);
	EnableWindow(m_wndBorderLabel.c, !m_bXPButtons);
}

BOOL CPage1::OnInit() 
{
  nsub = page1_init(this, c->sys, sub);
  m_sz.w = c->rc.r;
  m_sz.h = c->rc.b;
	if (1)
	{
		m_bXPButtons = FALSE;
		EnableWindow(m_wndXPButtons.c, FALSE);
	}
	else
	{
		EnableWindow(m_wndBorder.c, FALSE);
		EnableWindow(m_wndBorderLabel.c, FALSE);
	}

	m_Button.m_bTransparent = TRUE;

	OnResetButton();
	OnSetTooltip();
  
#if 0
	m_menu.LoadMenu (IDR_MENU1);
	m_btnMenu.m_hMenu = m_menu.GetSubMenu (0)->GetSafeHmenu ();
	m_btnMenu.SizeToContent ();
  m_btnMenu.m_bOSMenu = FALSE;
#endif

	m_btnCheck.SetImage(sys_find_img(c->sys, globalData.bIsOSAlphaBlendingSupport ? IDB_CHECKNO32 : IDB_CHECKNO));
	m_btnCheck.SetCheckedImage(sys_find_img(c->sys, globalData.bIsOSAlphaBlendingSupport ? IDB_CHECK32 : IDB_CHECK));
	m_btnCheck.m_nFlatStyle = BUTTONSTYLE_SEMIFLAT;

	m_btnRadio.m_nFlatStyle = BUTTONSTYLE_SEMIFLAT;
	m_btnRadio.SetImage(sys_find_img(c->sys, globalData.bIsOSAlphaBlendingSupport ? IDB_RADIO_OFF32 : IDB_RADIO_OFF));
	m_btnRadio.SetCheckedImage(sys_find_img(c->sys, globalData.bIsOSAlphaBlendingSupport ? IDB_RADIO_ON32 : IDB_RADIO_ON));

  // m_btnMultiLine.SizeToContent ();
	//CBCGPToolBar::AddToolBarForImageCollection (IDR_TOOLBAR_MENU_IMAGES);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CPage1::OnResetButton() 
{
	switch (m_iBorderStyle.GetCurSel())
	{
	case 0:
		m_Button.m_nFlatStyle = BUTTONSTYLE_FLAT;
		break;

	case 1:
		m_Button.m_nFlatStyle = BUTTONSTYLE_SEMIFLAT;
		break;

	case 2:
		m_Button.m_nFlatStyle = BUTTONSTYLE_3D;
	}

	if (m_iImage.GetCurSel() == 1)	// Text only
	{
		m_Button.SetImage(NULL);
	}
	else
	{
		if (globalData.bIsOSAlphaBlendingSupport)
		{
			m_Button.SetImage(sys_find_img(c->sys, IDB_BTN1_32), sys_find_img(c->sys, IDB_BTN1_HOT_32));
		}
		else
		{
			m_Button.SetImage (sys_find_img(c->sys, IDB_BTN1), sys_find_img(c->sys, IDB_BTN1_HOT));
		}
	}

	if (m_iImage.GetCurSel() == 0)
	{
		//SetWindowText(m_Button.c, _T(""));
	}
	else
	{
		//SetWindowText(m_Button.c, _T("BCGButton"));
	}

	switch (m_nImageLocation.GetCurSel())
	{
	case 0:
		m_Button.m_bRighImage = FALSE;
		m_Button.m_bTopImage = FALSE;
		break;

	case 1:
		m_Button.m_bRighImage = TRUE;
		m_Button.m_bTopImage = FALSE;
		break;

	case 2:
		m_Button.m_bRighImage = FALSE;
		m_Button.m_bTopImage = TRUE;
		break;
	}

	m_Button.SizeToContent ();
	Invalidate(c);
}

void CPage1::OnButton() 
{
	MessageBox (0, _T("Button Clicked!"), 0, 0);
}

void CPage1::OnSetTooltip() 
{
	//m_Button.SetTooltip (m_strToolTip.GetText());
}

void CPage1::OnPressedOnMenu() 
{
	
	//m_btnMenu.m_bStayPressed = m_bMenuStayPressed.GetCheck();
}

void CPage1::OnRightArrow() 
{
	
	//m_btnMenu.m_bRightArrow = m_bRightArrow.GetCheck();
	Invalidate(m_btnMenu.c);
}

void CPage1::OnButtonMenu() 
{
	CString strItem;
#if 0
	switch (m_btnMenu.m_nMenuResult)
	{
	case ID_ITEM_1:
		strItem = _T("Item 1");
		break;

	case ID_ITEM_2:
		strItem = _T("Item 2");
		break;

	case ID_ITEM_3:
		strItem = _T("Item 3");
		break;

	case ID_ITEM_4:
		strItem = _T("Item 4");
		break;

	default:
		if (!m_bMenuDefaultClick.GetCheck())
		{
			return;
		}

		strItem = _T("Default Menu Button Action");
		break;
	}

  MessageBox (strItem);
#endif
}

void CPage1::OnContextMenu(CWnd* /*pWnd*/, CPoint point) 
{
	CMenu menu;
  
  static menuitem_t menuitem[] = {
    0, ID_ITEM_1, -1, "Item &1",
    0, ID_ITEM_2, -1,  "Item &2",
    0, -1, -1,  0,
    0, ID_DIALOG_ABOUT, -1,  "&About...",
  };
	menu.LoadMenu(countof(menuitem), menuitem, c->sys);

  //ShowPopupMenu( menu.GetSubMenu (0)->GetSafeHmenu (), point.x, point.y, this, TRUE);
}

void CPage1::OnItem1() 
{
	MessageBox(0, _T("Command 1...."), 0, 0);
}

void CPage1::OnItem2() 
{
	MessageBox(0, _T("Command 2...."), 0, 0);
}

void CPage1::OnDialogAbout() 
{
	//BCGPShowAboutDlg (AFX_IDS_APP_TITLE);
}

void CPage1::OnMenuDefaultClick() 
{
#if 0
	m_btnMenu.m_bDefaultClick = m_bMenuDefaultClick.GetState();
  m_btnMenu.RedrawWindow ();
#endif
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE1_H__42746B5E_E6CC_48E5_BA21_103576DE34BE__INCLUDED_)

