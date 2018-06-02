// DiagOut.h : header file
//
#if !defined(AFX_DIAGOUT_H__C180A0DE_EAAC_41E1_B897_FDCEDAE4F7F2__INCLUDED_)
#define AFX_DIAGOUT_H__C180A0DE_EAAC_41E1_B897_FDCEDAE4F7F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDiagOut dialog pour le test et le debug

struct CDiagOut : public CDialog {
  // Construction
public:
  CDiagOut(const CString& text, CWnd* pParent = NULL);   // standard constructor

  // Dialog Data
  //{{AFX_DATA(CDiagOut)
  enum { IDD = IDD_DIAG_OUT };
  CString m_edit;
  //}}AFX_DATA


  // Overrides
  // ClassWizard generated function overrides
  //{{AFX_VIRTUAL(CDiagOut)
protected:
  void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CDiagOut)
  // NOTE: the ClassWizard will add member functions here
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIAGOUT_H__C180A0DE_EAAC_41E1_B897_FDCEDAE4F7F2__INCLUDED_)
