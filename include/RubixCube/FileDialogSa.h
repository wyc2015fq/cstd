// CFileDialogSa.h: interface for the CFileDialogSa struct.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_FILEDIALOGSA_H__F9923301_E2B2_4D6D_BF8E_75EF4FCCD918__INCLUDED_)
#define AFX_FILEDIALOGSA_H__F9923301_E2B2_4D6D_BF8E_75EF4FCCD918__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CFileDialogSa dialog

struct CFileDialogSa : public CFileDialog {
  DECLARE_DYNAMIC(CFileDialogSa)

public:
  CFileDialogSa(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
      LPCTSTR lpszFilter = NULL, LPCTSTR lpszFileName = NULL,
      DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
      CWnd* pParentWnd = NULL);

  // definition of the possible extensions and the default file name
  void SetNames(LPCTSTR lpszFilter = NULL, LPCTSTR lpszFileName = NULL);

  // reading from the extension by default selected
  inline CString GetDefExtension(BOOL point = FALSE) const {
    return (m_defExt[1] == 0 || point == FALSE) ? m_defExt + 1 : m_defExt;
  }


protected:
  static LPOFNHOOKPROC m_defHook;
  static void updExtension(OPENFILENAME* pOfn, int no);
  static UINT CALLBACK fnHook(HWND hdlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);

  TCHAR m_defExt[11];

  // void OnLBSelChangedNotify(UINT nIDBox, UINT iCurSel, UINT nCode);

  //{{AFX_MSG(CFileDialogSa)
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEDIALOGSA_H__F9923301_E2B2_4D6D_BF8E_75EF4FCCD918__INCLUDED_)
