#if !defined(AFX_STATICEX_H__543D545A_9F69_4F29_814B_D9A4C3EBA9EC__INCLUDED_)
#define AFX_STATICEX_H__543D545A_9F69_4F29_814B_D9A4C3EBA9EC__INCLUDED_

#include <Gdiplus.h>
#include "Extract.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StaticEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStaticEx window

struct CStaticEx : public CStatic {
  // Construction
public:
  CStaticEx();

  // Attributes
public:
  UINT m_count;
  UINT m_frameCount;
  INT m_width, m_height;
  int errors;
  BOOL chkHistory;
  bool acq;

  CExtract m_extract;

  // Operations
public:
  // void DeleteImages();
  // bool AddImage(struct Gdiplus::Bitmap *pBm);
  // void Comput();
  // void FileOpen(const CString &pathname);
protected:
  CFont m_font1, m_font2;

  // Operations
public:

  // Overrides
  // ClassWizard generated function overrides
  //{{AFX_VIRTUAL(CStaticEx)
protected:
  void PreSubclassWindow();
  //}}AFX_VIRTUAL

  // Implementation
public:
  ~CStaticEx();

  // Generated message map functions
protected:
  //{{AFX_MSG(CStaticEx)
  afx_msg void OnPaint();
  //}}AFX_MSG

  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATICEX_H__543D545A_9F69_4F29_814B_D9A4C3EBA9EC__INCLUDED_)
