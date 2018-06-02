// FileDialogSa.cpp : implementation file
//
//

#include "FileDialogSa.h"
#include <DLGS.H>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
// CFileDialogSa

LPOFNHOOKPROC CFileDialogSa::m_defHook = NULL;

IMPLEMENT_DYNAMIC(CFileDialogSa, CFileDialog)

static DWORD uiLBSELCHSTRING = 0;

CFileDialogSa::CFileDialogSa(BOOL bOpenFileDialog,
    LPCTSTR lpszFilter, LPCTSTR lpszFileName,
    DWORD dwFlags, CWnd* pParentWnd) :
  CFileDialog(bOpenFileDialog,
      m_defExt + 1, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
  _tcscpy(m_defExt, TEXT("."));
  m_defHook = m_ofn.lpfnHook;
  m_ofn.lpfnHook = fnHook;

  // create and register a new request
  uiLBSELCHSTRING = ::RegisterWindowMessage(LBSELCHSTRING);

  TRACE("CFileDialogSa() end %08x\n", uiLBSELCHSTRING);
}

void CFileDialogSa::SetNames(LPCTSTR lpszFilter, LPCTSTR lpszFileName)
{
  if (lpszFileName == NULL) {
    lpszFileName = "";
  }

  m_ofn.lpstrFilter = lpszFilter;
  lstrcpy(LPSTR(m_ofn.lpstrFile), lpszFileName);

  // Translate filter into commdlg format (lots of \0)
  if (lpszFilter) {
    m_strFilter = lpszFilter;
    LPTSTR pch = m_strFilter.GetBuffer(0); // modify the buffer in place

    // MFC delimits with '|' not '\0'
    while ((pch = _tcschr(pch, '|')) != NULL) {
      *pch++ = '\0';
    }

    m_ofn.lpstrFilter = m_strFilter;
    // do not call ReleaseBuffer() since the string contains '\0' characters
  }
}

// To add an extension on the file names
void CFileDialogSa::updExtension(OPENFILENAME* pOfn, int no)
{
  LPSTR pTmp, pFilter = LPSTR(pOfn->lpstrFilter);

  // the selected extension
  if (pFilter) {
    while (--no >= 0 && *pFilter) {
      pFilter += lstrlen(pFilter) + 1;
    }

    pFilter = strchr(pFilter, '.');
  }

  if (pFilter == NULL || pFilter[1] == '*') {
    pFilter = "\0";
  }

  // for add an extension on the file names
  if (pOfn->lpstrDefExt) {
    lstrcpy(LPSTR(pOfn->lpstrDefExt), pFilter + 1);
  }

  // only the file name without subdirectory
  pTmp = strrchr(pOfn->lpstrFile, '\\');

  if (pTmp == NULL) {
    pTmp = strrchr(pOfn->lpstrFile, ':');
  }

  if (pTmp) {
    lstrcpy(LPSTR(pOfn->lpstrFile), pTmp + 1);
  }

  // modify or add the extension
  pTmp = strrchr(pOfn->lpstrFile, '.');     // old extension ?

  if (pTmp) {
    lstrcpy(pTmp, pFilter);  // modify the extension
  }
  else {
    lstrcat(LPSTR(pOfn->lpstrFile), pFilter);  // or add a extension
  }
}

// Explorer-Style Hook Procedures
UINT CALLBACK CFileDialogSa::fnHook(HWND hdlg,// handle to dialog box window
    UINT uiMsg,      // message identifier
    WPARAM wParam,   // message parameter
    LPARAM lParam)   // message parameter
{
  static OPENFILENAME* pOfn = NULL;

  UINT ret = (m_defHook)(hdlg, uiMsg, wParam, lParam);

  if (uiMsg == WM_INITDIALOG) {
    pOfn = (OPENFILENAME*)lParam;
    updExtension(pOfn, 1);
    ::SetDlgItemText(::GetParent(hdlg), edt1, pOfn->lpstrFile);

  }
  else if (uiMsg == uiLBSELCHSTRING) {
    // dialog box for selecting the extension
    ::GetDlgItemText(::GetParent(hdlg), edt1, pOfn->lpstrFile, pOfn->nMaxFile);
    updExtension(pOfn, (lParam << 1) + 1);
    ::SetDlgItemText(::GetParent(hdlg), edt1, pOfn->lpstrFile);
  }

  TRACE("fnHook Msg=%08x wp=%08x lp=%08x %s\n",
      uiMsg, wParam, lParam, pOfn ? pOfn->lpstrFile : "");
  return ret;
}
