#if !defined(AFX_DIAGEXTRACT_H__7F0778A9_F721_4941_8E05_2957FD41C525__INCLUDED_)
#define AFX_DIAGEXTRACT_H__7F0778A9_F721_4941_8E05_2957FD41C525__INCLUDED_

#include "StaticEx.h"
#include "TwainCpp.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDiagExtr dialog

struct CDiagExtr : public CDialog, public CTwain {
  // Construction
public:
  CDiagExtr(CWnd* pParent = NULL);   // standard constructor
  CDiagExtr(const CString& fileName, CWnd* pParent = NULL);
  BOOL Create(CWnd* pParentWnd = NULL);

protected:
  // Dialog Data
  //{{AFX_DATA(CDiagExtr)
  enum { IDD = IDD_DIAG_EXTR };
  CStaticEx m_wnd;
  //}}AFX_DATA
  int m_error;

public:
  inline CString ToString() const {
    return m_wnd.m_extract.ToString();
  }
  inline bool IsReady() const {
    return m_wnd.m_extract.IsReady();
  }

  inline bool AddImage(struct Gdiplus::Bitmap* pBm) {
    bool cc = m_wnd.m_extract.AddImage(pBm);
    m_wnd.Invalidate();
    return cc;
  }

  inline void DeleteImages() {
    m_wnd.m_extract.DeleteImages();
    m_wnd.Invalidate();
  }

  inline bool IsBegin() const {
    return IsWindowVisible() && m_wnd.m_extract.IsBegin();
  }

  // TWAIN statements to instantiate the member of CTwain
  void GetIdentity();
  void CopyImage(HANDLE hBitmap, TW_IMAGEINFO& info);
protected:
  // Overrides
  // ClassWizard generated function overrides
  //{{AFX_VIRTUAL(CDiagExtr)
public:
  BOOL PreTranslateMessage(MSG* pMsg);
  void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  BOOL OnInitDialog();
  void OnOK();
  void OnCancel();
  //}}AFX_VIRTUAL

  // Implementation
  CWnd* m_pWnd;

  // Generated message map functions
  //{{AFX_MSG(CDiagExtr)
  afx_msg void OnChkhistory();
  afx_msg void OnTimer(UINT nIDEvent);
  afx_msg void OnResolver();
  afx_msg void OnAcquire();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIAGEXTRACT_H__7F0778A9_F721_4941_8E05_2957FD41C525__INCLUDED_)
