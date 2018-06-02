#if !defined(AFX_DIAGCTRL_H__3DAC83D4_D35C_11D3_A5EF_97CCD316C541__INCLUDED_)
#define AFX_DIAGCTRL_H__3DAC83D4_D35C_11D3_A5EF_97CCD316C541__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiagCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiagCtrl dialog

struct CDiagCtrl : public CDialog {
  // Construction
public:
  CDiagCtrl(CWnd* pParent = NULL);   // standard constructor

  // Dialog Data
  //{{AFX_DATA(CDiagCtrl)
  enum { IDD = IDD_DIAG_CTRL };
  CWnd*    m_pParent;
  int   m_plans;
  int   m_axes;
  //}}AFX_DATA


  // Overrides
  // ClassWizard generated function overrides
  //{{AFX_VIRTUAL(CDiagCtrl)
protected:
  void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CDiagCtrl)
  afx_msg void OnButtonMv(UINT id);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIAGCTRL_H__3DAC83D4_D35C_11D3_A5EF_97CCD316C541__INCLUDED_)
